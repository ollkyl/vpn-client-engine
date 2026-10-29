#include "killswitch.h"

#include <QApplication>
#include <QHostAddress>
#include <QRegularExpression>
#include <QSettings>
#include <algorithm>

#include "../client/core/utils/protocolEnum.h"
#include "../client/core/protocols/protocolUtils.h"
#include "../client/core/utils/constants/configKeys.h"
#include "../client/core/utils/constants/protocolConstants.h"
#include "qjsonarray.h"
#include "version.h"

static bool isValidIpOrCidr(const QString &value) {
    static const QRegularExpression re(
        QStringLiteral(R"(^(\d{1,3}\.){3}\d{1,3}(/\d{1,2})?$)"));
    if (!re.match(value).hasMatch()) return false;
    const QStringList ipParts = value.split(QLatin1Char('/'))[0].split(QLatin1Char('.'));
    for (const QString &part : ipParts) {
        bool ok;
        int octet = part.toInt(&ok);
        if (!ok || octet < 0 || octet > 255) return false;
    }
    if (value.contains(QLatin1Char('/'))) {
        bool ok;
        int prefix = value.split(QLatin1Char('/'))[1].toInt(&ok);
        if (!ok || prefix < 0 || prefix > 32) return false;
    }
    return true;
}

#include "../client/platforms/windows/daemon/windowsfirewall.h"
#include "../client/platforms/windows/daemon/windowsdaemon.h"

KillSwitch* s_instance = nullptr;

KillSwitch* KillSwitch::instance()
{
    if (s_instance == nullptr) {
        s_instance = new KillSwitch(qApp);
    }
    return s_instance;
}

bool KillSwitch::init()
{
    if (isStrictKillSwitchEnabled()) {
        return disableAllTraffic();
    }

    return true;
}

bool KillSwitch::refresh(bool enabled)
{
    QSettings RegHLM("HKEY_LOCAL_MACHINE\\Software\\" + QString(ORGANIZATION_NAME)
                             + "\\" + QString(APPLICATION_NAME), QSettings::NativeFormat);
    RegHLM.setValue("strictKillSwitchEnabled", enabled);

    if (isStrictKillSwitchEnabled()) {
        return disableAllTraffic();
    }  else {
        return disableKillSwitch();
    }

}

bool KillSwitch::isStrictKillSwitchEnabled()
{
    QSettings RegHLM("HKEY_LOCAL_MACHINE\\Software\\" + QString(ORGANIZATION_NAME)
                             + "\\" + QString(APPLICATION_NAME), QSettings::NativeFormat);
    return RegHLM.value("strictKillSwitchEnabled", false).toBool();
}

bool KillSwitch::disableKillSwitch() {

    if (isStrictKillSwitchEnabled()) {
        return disableAllTraffic();
    }
    return WindowsFirewall::create(this)->allowAllTraffic();

    m_allowedRanges.clear();
    return true;
}

bool KillSwitch::disableAllTraffic() {
    WindowsFirewall::create(this)->enableInterface(-1);
    m_allowedRanges.clear();
    return true;
}

bool KillSwitch::resetAllowedRange(const QStringList &ranges) {

    if (!std::all_of(ranges.cbegin(), ranges.cend(), isValidIpOrCidr)) {
        qCritical() << "IPC: invalid IP/CIDR in ranges, rejecting resetAllowedRange";
        return false;
    }
    m_allowedRanges = ranges;

    if (isStrictKillSwitchEnabled()) {
        WindowsFirewall::create(this)->enableInterface(-1);
    }
    WindowsFirewall::create(this)->allowTrafficRange(m_allowedRanges);

    return true;
}

bool KillSwitch::addAllowedRange(const QStringList &ranges) {
    if (!std::all_of(ranges.cbegin(), ranges.cend(), isValidIpOrCidr)) {
        qCritical() << "IPC: invalid IP/CIDR in ranges, rejecting addAllowedRange";
        return false;
    }
    for (const QString &range : ranges) {
        if (!range.isEmpty() && !m_allowedRanges.contains(range)) {
            m_allowedRanges.append(range);
        }
    }

    return resetAllowedRange(m_allowedRanges);
}

bool KillSwitch::enablePeerTraffic(const QJsonObject &configStr) {
    InterfaceConfig config;

    config.m_primaryDnsServer = configStr.value(amnezia::configKey::dns1).toString();

    // We don't use secondary DNS if primary DNS is AmneziaDNS
    if (!config.m_primaryDnsServer.contains(amnezia::protocols::dns::amneziaDnsIp)) {
        config.m_secondaryDnsServer = configStr.value(amnezia::configKey::dns2).toString();
    }

    config.m_serverPublicKey = "openvpn";
    config.m_serverIpv4Gateway = configStr.value("vpnGateway").toString();
    config.m_serverIpv4AddrIn = configStr.value("vpnServer").toString();
    int vpnAdapterIndex = configStr.value("vpnAdapterIndex").toInt();
    int inetAdapterIndex = configStr.value("inetAdapterIndex").toInt();

    int splitTunnelType = configStr.value("splitTunnelType").toInt();
    QJsonArray splitTunnelSites = configStr.value("splitTunnelSites").toArray();

    // Use APP split tunnel
    if (splitTunnelType == 0 || splitTunnelType == 2) {
        config.m_allowedIPAddressRanges.append(IPAddress(QHostAddress("0.0.0.0"), 0));
        config.m_allowedIPAddressRanges.append(IPAddress(QHostAddress("::"), 0));
    }

    if (splitTunnelType == 1) {
        for (auto v : splitTunnelSites) {
            QString ipRange = v.toString();
            if (ipRange.split('/').size() > 1) {
                config.m_allowedIPAddressRanges.append(
                        IPAddress(QHostAddress(ipRange.split('/')[0]), atoi(ipRange.split('/')[1].toLocal8Bit())));
            } else {
                config.m_allowedIPAddressRanges.append(IPAddress(QHostAddress(ipRange), 32));
            }
        }
    }

    config.m_excludedAddresses.append(configStr.value("vpnServer").toString());
    if (splitTunnelType == 2) {
        for (auto v : splitTunnelSites) {
            QString ipRange = v.toString();
            config.m_excludedAddresses.append(ipRange);
        }
    }

    for (const QJsonValue &i : configStr.value(amnezia::configKey::splitTunnelApps).toArray()) {
        if (!i.isString()) {
            break;
        }
        config.m_vpnDisabledApps.append(i.toString());
    }

    for (auto dns : configStr.value(amnezia::configKey::allowedDnsServers).toArray()) {
        if (!dns.isString()) {
            break;
        }
        config.m_allowedDnsServers.append(dns.toString());
    }

    // killSwitch toggle
    if (QVariant(configStr.value(amnezia::configKey::killSwitchOption).toString()).toBool()) {
        WindowsFirewall::create(this)->enablePeerTraffic(config);
    }

    WindowsDaemon::instance()->prepareActivation(config, inetAdapterIndex);
    WindowsDaemon::instance()->activateSplitTunnel(config, vpnAdapterIndex);
    return true;
}

bool KillSwitch::enableKillSwitch(const QJsonObject &configStr, int vpnAdapterIndex) {
    if (configStr.value("splitTunnelType").toInt() != 0) {
        WindowsFirewall::create(this)->allowAllTraffic();
    }
    return WindowsFirewall::create(this)->enableInterface(vpnAdapterIndex);

    return true;
}
