#include "serverConfigUtils.h"

#include <QJsonArray>
#include <QJsonValue>

#include "core/utils/constants/configKeys.h"

namespace
{
    // configVersion values used by the upstream Amnezia subscription API
    constexpr int apiConfigSourceTelegram = 1;
    constexpr int apiConfigSourceGateway = 2;
}

namespace serverConfigUtils
{

bool isNativeConfig(const QJsonObject &serverConfigObject)
{
    if (isServerFromApi(serverConfigObject)) {
        return false;
    }

    const QJsonArray containersArray = serverConfigObject.value(amnezia::configKey::containers).toArray();
    for (const QJsonValue &val : containersArray) {
        const QJsonObject containerObj = val.toObject();
        for (auto it = containerObj.begin(); it != containerObj.end(); ++it) {
            if (it.key() == amnezia::configKey::container) {
                continue;
            }
            if (it.value().toObject().value(amnezia::configKey::isThirdPartyConfig).toBool()) {
                return true;
            }
        }
    }
    return false;
}

bool isServerFromApi(const QJsonObject &serverConfigObject)
{
    const int configVersion = serverConfigObject.value(amnezia::configKey::configVersion).toInt();
    return configVersion == apiConfigSourceTelegram || configVersion == apiConfigSourceGateway;
}

} // namespace serverConfigUtils
