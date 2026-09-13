#ifndef SERVERDESCRIPTION_H
#define SERVERDESCRIPTION_H

#include <QString>
#include <QJsonArray>

#include "core/utils/containerEnum.h"
#include "core/utils/selfhosted/sshSession.h"
#include "core/models/selfhosted/nativeServerConfig.h"

namespace amnezia
{

struct ServerDescription
{
    QString serverId;

    QString serverName;
    QString baseDescription;
    QString hostName;

    int configVersion = 0;

    ServerCredentials selfHostedSshCredentials;
    bool hasWriteAccess = false;

    bool primaryDnsIsAmnezia = false;
    DockerContainer defaultContainer = DockerContainer::None;
    bool hasInstalledVpnContainers = false;

    // NOTE: this is the public "engine" tree - the upstream Amnezia gateway-API fields
    // (isApiV1/isApiV2, premium/subscription state, ad label content, country selection)
    // were removed here: buildServerDescription() below never populated them for a
    // self-hosted/imported server, so every reader of them was permanently dead code.
    // isServerFromGatewayApi is kept because it's still read by PageSettingsDns.qml and
    // PageSettingsSplitTunneling.qml (it's just always false without a gateway API client).
    bool isServerFromGatewayApi = false;

    QString collapsedServerDescription;
    QString expandedServerDescription;
};

ServerDescription buildServerDescription(const NativeServerConfig &server, bool isAmneziaDnsEnabled);

} // namespace amnezia

#endif
