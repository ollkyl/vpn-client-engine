#include "serverDescription.h"

#include "core/utils/containers/containerUtils.h"
#include "core/models/protocols/awgProtocolConfig.h"

using namespace amnezia;

namespace
{

QString protocolName(DockerContainer defaultContainer, const QMap<DockerContainer, ContainerConfig> &containers)
{
    QString name = ContainerUtils::containerHumanNames().value(defaultContainer);
    if (!ContainerUtils::isAwgContainer(defaultContainer)) {
        return name;
    }

    const auto it = containers.constFind(defaultContainer);
    if (it == containers.cend()) {
        return name;
    }
    const AwgProtocolConfig *awg = it->getAwgProtocolConfig();
    if (!awg) {
        return name;
    }

    QString version = awg->clientProtocolVersion();
    if (version.isEmpty()) {
        version = awg->serverProtocolVersion();
    }
    return name + AwgProtocolConfig::protocolVersionString(version);
}

} // namespace

namespace amnezia
{

ServerDescription buildServerDescription(const QString &serverId, const NativeServerConfig &server)
{
    ServerDescription d;
    d.serverId = serverId;
    d.name = server.displayName;
    d.hostName = server.hostName;
    d.defaultContainer = server.defaultContainer;
    d.containers = server.containers.keys();
    d.protocolName = protocolName(server.defaultContainer, server.containers);
    return d;
}

} // namespace amnezia
