#ifndef SERVERDESCRIPTION_H
#define SERVERDESCRIPTION_H

#include <QList>
#include <QString>

#include "core/utils/containerEnum.h"
#include "core/models/nativeServerConfig.h"

namespace amnezia
{

// Read-only summary of a stored server, intended for listing servers in a client UI
struct ServerDescription
{
    QString serverId;
    QString name;
    QString hostName;
    DockerContainer defaultContainer = DockerContainer::None;
    QList<DockerContainer> containers;
    // Human readable name of the default container's protocol, e.g. "AmneziaWG 2.0"
    QString protocolName;
};

ServerDescription buildServerDescription(const QString &serverId, const NativeServerConfig &server);

} // namespace amnezia

#endif
