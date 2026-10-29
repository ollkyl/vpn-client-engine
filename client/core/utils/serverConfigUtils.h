#ifndef SERVERCONFIGUTILS_H
#define SERVERCONFIGUTILS_H

#include <QJsonObject>

namespace serverConfigUtils
{

// True for a server built from an imported client config (WireGuard/AWG/OpenVPN/XRay/... file or vpn:// key).
// Only such servers can be stored and connected by the engine.
bool isNativeConfig(const QJsonObject &serverConfigObject);

// True for configs issued by the upstream Amnezia subscription API; the engine rejects them on import.
bool isServerFromApi(const QJsonObject &serverConfigObject);

} // namespace serverConfigUtils

#endif // SERVERCONFIGUTILS_H
