#include "serverDescription.h"

#include <QMap>

#include "core/utils/serverConfigUtils.h"
#include "core/utils/constants/apiKeys.h"
#include "core/utils/constants/apiConstants.h"
#include "core/utils/constants/protocolConstants.h"
#include "core/utils/containers/containerUtils.h"
#include "core/protocols/protocolUtils.h"
#include "core/models/protocols/awgProtocolConfig.h"

using namespace amnezia;

namespace
{

bool computeHasInstalledVpnContainers(const QMap<DockerContainer, ContainerConfig> &containers)
{
    for (auto it = containers.begin(); it != containers.end(); ++it) {
        const DockerContainer container = it.key();
        if (ContainerUtils::containerService(container) == ServiceType::Vpn || container == DockerContainer::SSXray) {
            return true;
        }
    }
    return false;
}

template <typename T>
ServerDescription buildBaseDescription(const T &server)
{
    ServerDescription row;
    row.hostName = server.hostName;
    row.defaultContainer = server.defaultContainer;
    row.primaryDnsIsAmnezia = (server.dns1 == protocols::dns::amneziaDnsIp);
    row.hasInstalledVpnContainers = computeHasInstalledVpnContainers(server.containers);
    return row;
}

QString getBaseDescription(const QMap<DockerContainer, ContainerConfig> &containers,
                         bool isAmneziaDnsEnabled,
                         bool hasWriteAccess,
                         bool primaryDnsIsAmnezia)
{
    QString description;
    if (hasWriteAccess) {
        const bool isDnsInstalled = containers.contains(DockerContainer::Dns);
        if (isAmneziaDnsEnabled && isDnsInstalled) {
            description += QStringLiteral("Amnezia DNS | ");
        }
    } else if (primaryDnsIsAmnezia) {
        description += QStringLiteral("Amnezia DNS | ");
    }
    return description;
}

QString getProtocolName(DockerContainer defaultContainer, const QMap<DockerContainer, ContainerConfig> &containers)
{
    QString containerName = ContainerUtils::containerHumanNames().value(defaultContainer);
    QString protocolVersion;

    if (ContainerUtils::isAwgContainer(defaultContainer)) {
        const auto it = containers.constFind(defaultContainer);
        if (it != containers.cend()) {
            if (const AwgProtocolConfig *awg = it->getAwgProtocolConfig()) {
                QString version = awg->clientProtocolVersion();
                if (version.isEmpty()) {
                    version = awg->serverProtocolVersion();
                }
                protocolVersion = AwgProtocolConfig::protocolVersionString(version);
                if (defaultContainer == DockerContainer::Awg && !awg->serverConfig.isThirdPartyConfig) {
                    containerName = QStringLiteral("AmneziaWG Legacy");
                }
            }
        }
    }

    return containerName + protocolVersion + QStringLiteral(" | ");
}

} // namespace

namespace amnezia
{

ServerDescription buildServerDescription(const NativeServerConfig &server, bool isAmneziaDnsEnabled)
{
    ServerDescription row = buildBaseDescription(server);
    row.hasWriteAccess = false;

    row.serverName = server.displayName;
    row.baseDescription = getBaseDescription(server.containers, isAmneziaDnsEnabled, row.hasWriteAccess, row.primaryDnsIsAmnezia);

    const QString protocolName = getProtocolName(server.defaultContainer, server.containers);
    row.expandedServerDescription = row.baseDescription + row.hostName;
    row.collapsedServerDescription = row.baseDescription + protocolName + row.hostName;
    return row;
}

} // namespace amnezia
