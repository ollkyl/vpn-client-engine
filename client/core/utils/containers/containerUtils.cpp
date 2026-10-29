#include "containerUtils.h"

#include <QMetaEnum>
#include <QObject>
#include <QJsonDocument>

using namespace amnezia;

DockerContainer ContainerUtils::containerFromString(const QString &container)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<DockerContainer>();
    for (int i = 0; i < metaEnum.keyCount(); ++i) {
        DockerContainer c = static_cast<DockerContainer>(i);
        if (container == containerToString(c))
            return c;
    }
    return DockerContainer::None;
}

QString ContainerUtils::containerToString(DockerContainer c)
{
    if (c == DockerContainer::None)
        return "none";
    if (c == DockerContainer::Cloak)
        return "amnezia-openvpn-cloak";
    if (c == DockerContainer::Awg)
        return "amnezia-awg";
    if (c == DockerContainer::Awg2)
        return "amnezia-awg2";
    QMetaEnum metaEnum = QMetaEnum::fromType<DockerContainer>();
    QString containerKey = metaEnum.valueToKey(static_cast<int>(c));

    return "amnezia-" + containerKey.toLower();
}

QString ContainerUtils::containerTypeToString(DockerContainer c)
{
    if (c == DockerContainer::None)
        return "none";
    if (c == DockerContainer::Ipsec)
        return "ikev2";
    if (c == DockerContainer::Awg)
        return "awg";
    if (c == DockerContainer::Awg2)
        return "awg";
    QMetaEnum metaEnum = QMetaEnum::fromType<DockerContainer>();
    QString containerKey = metaEnum.valueToKey(static_cast<int>(c));

    return containerKey.toLower();
}

QList<DockerContainer> ContainerUtils::allContainers()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<DockerContainer>();
    QList<DockerContainer> all;
    for (int i = 0; i < metaEnum.keyCount(); ++i) {
        all.append(static_cast<DockerContainer>(i));
    }

    return all;
}

QMap<DockerContainer, QString> ContainerUtils::containerHumanNames()
{
    return { { DockerContainer::None, "Not installed" },
             { DockerContainer::OpenVpn, "OpenVPN" },
             { DockerContainer::ShadowSocks, "OpenVPN over SS" },
             { DockerContainer::Cloak, "OpenVPN over Cloak" },
             { DockerContainer::WireGuard, "WireGuard" },
             { DockerContainer::Awg, "AmneziaWG" },
             { DockerContainer::Awg2, "AmneziaWG" },
             { DockerContainer::Xray, "XRay" },
             { DockerContainer::Ipsec, "IPsec" },
             { DockerContainer::SSXray, "Shadowsocks" } };
}

ServiceType ContainerUtils::containerService(DockerContainer c)
{
    if (isUnsupportedContainer(c)) {
        return ServiceType::Vpn;
    }
    return ProtocolUtils::protocolService(defaultProtocol(c));
}

Proto ContainerUtils::defaultProtocol(DockerContainer c)
{
    switch (c) {
    case DockerContainer::None: return Proto::Unknown;
    case DockerContainer::OpenVpn: return Proto::OpenVpn;
    case DockerContainer::Cloak:
    case DockerContainer::ShadowSocks: return Proto::Unknown;
    case DockerContainer::WireGuard: return Proto::WireGuard;
    case DockerContainer::Awg2: return Proto::Awg;
    case DockerContainer::Awg: return Proto::Awg;
    case DockerContainer::Xray: return Proto::Xray;
    case DockerContainer::Ipsec: return Proto::Ikev2;
    case DockerContainer::SSXray: return Proto::SSXray;
    default: return Proto::Unknown;
    }
}

QString ContainerUtils::containerTypeToProtocolString(DockerContainer c)
{
    if (c == DockerContainer::None)
        return "none";

    Proto p = defaultProtocol(c);
    return ProtocolUtils::protoToString(p);
}

bool ContainerUtils::isSupportedByCurrentPlatform(DockerContainer c)
{
    Q_UNUSED(c)
    return true;
}

bool ContainerUtils::isAwgContainer(DockerContainer container)
{
    return container == DockerContainer::Awg || container == DockerContainer::Awg2;
}

bool ContainerUtils::isUnsupportedContainer(DockerContainer container)
{
    return container == DockerContainer::Cloak || container == DockerContainer::ShadowSocks;
}

QJsonObject ContainerUtils::getProtocolConfigFromContainer(const Proto protocol, const QJsonObject &containerConfig)
{
    QString protocolConfigString = containerConfig.value(ProtocolUtils::protoToString(protocol))
    .toObject()
            .value(configKey::lastConfig)
            .toString();

    return QJsonDocument::fromJson(protocolConfigString.toUtf8()).object();
}
