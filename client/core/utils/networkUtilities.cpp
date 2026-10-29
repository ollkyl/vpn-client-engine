#include "networkUtilities.h"
#include <QtNetwork/qnetworkinterface.h>
#include <cstddef>

#include <windows.h>
#include <Ipexport.h>
#include <Ws2tcpip.h>
#include <ws2ipdef.h>
#include <Iphlpapi.h>
#include <Iptypes.h>
#include <WinSock2.h>
#include <winsock.h>
#include <QNetworkInterface>
#include "qendian.h"
#include <QSettings>

#include <QHostAddress>
#include <QHostInfo>

QRegExp NetworkUtilities::ipAddressWithSubnetRegExp()
{
    return QRegExp("(?:(?:25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\\.){3}"
                   "(?:25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])"
                   "(\\/(?:[0-9]|[1-2][0-9]|3[0-2])){0,1}");
}

QString NetworkUtilities::netMaskFromIpWithSubnet(const QString ip)
{
    if (!ip.contains("/"))
        return "255.255.255.255";

    const QStringList parts = ip.split("/");
    if (parts.size() < 2)
        return "255.255.255.255";

    bool ok;
    int prefix = parts.at(1).toInt(&ok);
    if (!ok || prefix < 0 || prefix > 32)
        return "255.255.255.255";

    const quint32 mask = prefix == 0 ? 0 : 0xFFFFFFFFu << (32 - prefix);

    return QString("%1.%2.%3.%4").arg(mask >> 24).arg((mask >> 16) & 0xFF).arg((mask >> 8) & 0xFF).arg(mask & 0xFF);
}

QString NetworkUtilities::ipAddressFromIpWithSubnet(const QString ip)
{
    if (ip.count(".") != 3)
        return "";
    return ip.split("/").first();
}

QString NetworkUtilities::getIPAddress(const QString &host)
{
    QHostAddress address(host);
    if (QAbstractSocket::IPv4Protocol == address.protocol()) {
        return host;
    } else if (QAbstractSocket::IPv6Protocol == address.protocol()) {
        return host;
    }

    QList<QHostAddress> addresses = QHostInfo::fromName(host).addresses();
    if (!addresses.isEmpty()) {
        return addresses.first().toString();
    }
    qDebug() << "Unable to resolve address for " << host;
    return "";
}

bool NetworkUtilities::checkIPv4Format(const QString &ip)
{
    if (ip.isEmpty())
        return false;
    int count = ip.count(".");
    if (count != 3)
        return false;

    QHostAddress addr(ip);
    return (addr.protocol() == QAbstractSocket::NetworkLayerProtocol::IPv4Protocol);
}

bool NetworkUtilities::checkIpSubnetFormat(const QString &ip)
{
    if (!ip.contains("/"))
        return checkIPv4Format(ip);

    QStringList parts = ip.split("/");
    if (parts.size() != 2)
        return false;

    bool ok;
    int subnet = parts.at(1).toInt(&ok);
    if (subnet >= 0 && subnet <= 32 && ok)
        return checkIPv4Format(parts.at(0));
    else
        return false;
}

// static
int NetworkUtilities::AdapterIndexTo(const QHostAddress& dst) {
    qDebug() << "Getting Current Internet Adapter that routes to"
             << dst.toString();
    quint32 ipBigEndian;
    quint32 ip = dst.toIPv4Address();
    qToBigEndian(ip, &ipBigEndian);
    _MIB_IPFORWARDROW routeInfo;
    auto result = GetBestRoute(ipBigEndian, 0, &routeInfo);
    if (result != NO_ERROR) {
        return -1;
    }
    auto adapter =
        QNetworkInterface::interfaceFromIndex(routeInfo.dwForwardIfIndex);
    qDebug() << "Internet Adapter:" << adapter.name();
    return routeInfo.dwForwardIfIndex;
    return 0;
}

DWORD GetAdaptersAddressesWrapper(const ULONG Family,
                                  const ULONG Flags,
                                  const PVOID Reserved,
                                  _Out_ PIP_ADAPTER_ADDRESSES& pAdapterAddresses) {
    DWORD dwRetVal = 0;
    int iter = 0;
    constexpr int max_iter = 3;
    ULONG AdapterAddressesLen = 15000;
    do {
        // xassert2(pAdapterAddresses == nullptr);
        pAdapterAddresses = (IP_ADAPTER_ADDRESSES*)malloc(AdapterAddressesLen);
        if (pAdapterAddresses == nullptr) {
            qDebug() << "can not malloc" << AdapterAddressesLen << "bytes";
            return ERROR_OUTOFMEMORY;
        }

        dwRetVal = GetAdaptersAddresses(Family, Flags, NULL, pAdapterAddresses, &AdapterAddressesLen);

        if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
            free(pAdapterAddresses);
            pAdapterAddresses = nullptr;
        } else {
            break;
        }

        iter++;
    } while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (iter < max_iter));

    if (dwRetVal != NO_ERROR) {
        qDebug() << "Family: " << Family << ", Flags: " << Flags << " AdapterAddressesLen: " << AdapterAddressesLen <<
                ", dwRetVal:" << dwRetVal << ", iter: " << iter;
        if (pAdapterAddresses) {
            free(pAdapterAddresses);
            pAdapterAddresses = nullptr;
        }
    }

    return dwRetVal;
}

QPair<QString, QNetworkInterface> NetworkUtilities::getGatewayAndIface()
{
    constexpr int BUFF_LEN = 100;
    char buff[BUFF_LEN] = {'\0'};

    QString resGateway;
    int resIndex = -1;

    PIP_ADAPTER_ADDRESSES pAdapterAddresses = nullptr;
    DWORD dwRetVal =
            GetAdaptersAddressesWrapper(AF_INET, GAA_FLAG_INCLUDE_GATEWAYS, NULL, pAdapterAddresses);

    if (dwRetVal != NO_ERROR) {
        qDebug() << "ipv4 stack detect GetAdaptersAddresses failed.";
        return {};
    }

    PIP_ADAPTER_ADDRESSES pCurAddress = pAdapterAddresses;
    while (pCurAddress) {
        PIP_ADAPTER_GATEWAY_ADDRESS_LH gateway = pCurAddress->FirstGatewayAddress;
        if (gateway) {
            SOCKET_ADDRESS gateway_address = gateway->Address;
            if (gateway->Address.lpSockaddr->sa_family == AF_INET) {
                sockaddr_in* sa_in = (sockaddr_in*)gateway->Address.lpSockaddr;
                QString gw = inet_ntop(AF_INET, &(sa_in->sin_addr), buff, BUFF_LEN);
                qDebug() <<  "gateway IPV4:" << gw;
                struct sockaddr_in addr;
                if (inet_pton(AF_INET, buff, &addr.sin_addr) == 1) {
                    qDebug() <<  "this is true v4 !";
                    
                    resGateway = gw;
                    resIndex = pCurAddress->IfIndex;
                }
            }
        }
        pCurAddress = pCurAddress->Next;
    }

    free(pAdapterAddresses);
    return { resGateway, QNetworkInterface::interfaceFromIndex(resIndex) };
}
