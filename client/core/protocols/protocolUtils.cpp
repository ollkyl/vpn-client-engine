#include "protocolUtils.h"

#include <QJsonObject>
#include <QObject>

using namespace amnezia;

QList<Proto> ProtocolUtils::allProtocols()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Proto>();
    QList<Proto> all;
    for (int i = 0; i < metaEnum.keyCount(); ++i) {
        all.append(static_cast<Proto>(i));
    }

    return all;
}

TransportProto ProtocolUtils::transportProtoFromString(QString p)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<TransportProto>();
    for (int i = 0; i < metaEnum.keyCount(); ++i) {
        TransportProto tp = static_cast<TransportProto>(i);
        if (p.toLower() == transportProtoToString(tp).toLower())
            return tp;
    }
    return TransportProto::Udp;
}

QString ProtocolUtils::transportProtoToString(TransportProto proto, Proto p)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<TransportProto>();
    QString protoKey = metaEnum.valueToKey(static_cast<int>(proto));
    return protoKey.toLower();
}

Proto ProtocolUtils::protoFromString(QString proto)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Proto>();
    for (int i = 0; i < metaEnum.keyCount(); ++i) {
        Proto p = static_cast<Proto>(i);
        if (proto == protoToString(p))
            return p;
    }
    return Proto::Unknown;
}

QString ProtocolUtils::protoToString(Proto p)
{
    if (p == Proto::Unknown)
        return "";

    QMetaEnum metaEnum = QMetaEnum::fromType<Proto>();
    QString protoKey = metaEnum.valueToKey(static_cast<int>(p));
    return protoKey.toLower();
}

QMap<Proto, QString> ProtocolUtils::protocolHumanNames()
{
    return { { Proto::OpenVpn, "OpenVPN" },
             { Proto::WireGuard, "WireGuard" },
             { Proto::Awg, "AmneziaWG" },
             { Proto::Ikev2, "IKEv2" },
             { Proto::Xray, "XRay" },
             { Proto::SSXray, "Shadowsocks" } };
}

ServiceType ProtocolUtils::protocolService(Proto p)
{
    switch (p) {
    case Proto::OpenVpn:
    case Proto::WireGuard:
    case Proto::Awg:
    case Proto::Ikev2:
    case Proto::Xray: return ServiceType::Vpn;
    default: return ServiceType::None;
    }
}

int ProtocolUtils::defaultPort(Proto p)
{
    switch (p) {
    case Proto::OpenVpn: return QString(protocols::openvpn::defaultPort).toInt();
    case Proto::WireGuard: return QString(protocols::wireguard::defaultPort).toInt();
    case Proto::Awg: return QString(protocols::awg::defaultPort).toInt();
    case Proto::Xray: return QString(protocols::xray::defaultPort).toInt();
    default: return -1;
    }
}

TransportProto ProtocolUtils::defaultTransportProto(Proto p)
{
    switch (p) {
    case Proto::Xray:
    case Proto::SSXray: return TransportProto::Tcp;
    default: return TransportProto::Udp;
    }
}

QString ProtocolUtils::key_proto_config_data(Proto p)
{
    return protoToString(p) + "_config_data";
}

QString ProtocolUtils::key_proto_config_path(Proto p)
{
    return protoToString(p) + "_config_path";
}

