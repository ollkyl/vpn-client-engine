#include "serversController.h"
#include "core/utils/serverConfigUtils.h"
#include "core/utils/protocolEnum.h"
#include "core/protocols/protocolUtils.h"
#include "core/utils/constants/configKeys.h"
#include "core/models/containerConfig.h"

#include "core/models/serverDescription.h"

#if defined(Q_OS_IOS) || defined(MACOS_NE)
    #include "core/utils/swiftBridge.h"
#endif


ServersController::ServersController(SecureServersRepository *serversRepository,
                                      SecureAppSettingsRepository *appSettingsRepository, QObject *parent)
    : QObject(parent), m_serversRepository(serversRepository), m_appSettingsRepository(appSettingsRepository)
{
    ensureDefaultServerValid();
}

void ServersController::ensureDefaultServerValid()
{
    if (!getServersCount()) {
        return;
    }

    const QString defaultId = getDefaultServerId();
    if (!defaultId.isEmpty() && indexOfServerId(defaultId) >= 0) {
        return;
    }

    const QString firstId = getServerId(0);
    if (!firstId.isEmpty()) {
        setDefaultServer(firstId);
    }
}

bool ServersController::renameServer(const QString &serverId, const QString &name)
{
    const serverConfigUtils::ConfigType kind = m_serversRepository->serverKind(serverId);
    switch (kind) {
    case serverConfigUtils::ConfigType::Native: {
        auto cfg = m_serversRepository->nativeConfig(serverId);
        if (!cfg.has_value()) return false;
        cfg->description = name;
        cfg->displayName = name;
        m_serversRepository->editServer(serverId, cfg->toJson(), kind);
        return true;
    }
    case serverConfigUtils::ConfigType::AmneziaPremiumV2:
    case serverConfigUtils::ConfigType::AmneziaFreeV3:
    case serverConfigUtils::ConfigType::ExternalPremium:
    case serverConfigUtils::ConfigType::AmneziaPremiumV1:
    case serverConfigUtils::ConfigType::AmneziaFreeV2:
    case serverConfigUtils::ConfigType::SelfHostedAdmin:
    case serverConfigUtils::ConfigType::SelfHostedUser:
    case serverConfigUtils::ConfigType::Invalid:
    default:
        return false;
    }
}

void ServersController::removeServer(const QString &serverId)
{
    m_serversRepository->removeServer(serverId);
}

void ServersController::setDefaultServer(const QString &serverId)
{
    m_serversRepository->setDefaultServer(serverId);
}

void ServersController::setDefaultContainer(const QString &serverId, DockerContainer container)
{
    const serverConfigUtils::ConfigType kind = m_serversRepository->serverKind(serverId);
    switch (kind) {
    case serverConfigUtils::ConfigType::Native: {
        auto cfg = m_serversRepository->nativeConfig(serverId);
        if (!cfg.has_value()) return;
        cfg->defaultContainer = container;
        m_serversRepository->editServer(serverId, cfg->toJson(), kind);
        return;
    }
    case serverConfigUtils::ConfigType::AmneziaPremiumV2:
    case serverConfigUtils::ConfigType::AmneziaFreeV3:
    case serverConfigUtils::ConfigType::ExternalPremium:
    case serverConfigUtils::ConfigType::AmneziaPremiumV1:
    case serverConfigUtils::ConfigType::AmneziaFreeV2:
    case serverConfigUtils::ConfigType::SelfHostedAdmin:
    case serverConfigUtils::ConfigType::SelfHostedUser:
    case serverConfigUtils::ConfigType::Invalid:
    default:
        return;
    }
}

QVector<ServerDescription> ServersController::buildServerDescriptions(bool isAmneziaDnsEnabled) const
{
    QVector<ServerDescription> out;
    const QVector<QString> ids = m_serversRepository->orderedServerIds();
    out.reserve(ids.size());

    for (const QString &id : ids) {
        ServerDescription d;
        using Kind = serverConfigUtils::ConfigType;
        const Kind kind = m_serversRepository->serverKind(id);
        switch (kind) {
        case Kind::Native: {
            const auto cfg = m_serversRepository->nativeConfig(id);
            if (!cfg) {
                continue;
            }
            d = buildServerDescription(*cfg, isAmneziaDnsEnabled);
            break;
        }
        case Kind::AmneziaPremiumV2:
        case Kind::AmneziaFreeV3:
        case Kind::ExternalPremium:
        case Kind::AmneziaPremiumV1:
        case Kind::AmneziaFreeV2:
        case Kind::SelfHostedAdmin:
        case Kind::SelfHostedUser:
        case Kind::Invalid:
        default:
            continue;
        }

        d.serverId = id;
        out.append(d);
    }
    return out;
}

QMap<DockerContainer, ContainerConfig> ServersController::getServerContainersMap(const QString &serverId) const
{
    switch (m_serversRepository->serverKind(serverId)) {
    case serverConfigUtils::ConfigType::Native: {
        const auto cfg = m_serversRepository->nativeConfig(serverId);
        return cfg.has_value() ? cfg->containers : QMap<DockerContainer, ContainerConfig>{};
    }
    case serverConfigUtils::ConfigType::AmneziaPremiumV2:
    case serverConfigUtils::ConfigType::AmneziaFreeV3:
    case serverConfigUtils::ConfigType::ExternalPremium:
    case serverConfigUtils::ConfigType::AmneziaPremiumV1:
    case serverConfigUtils::ConfigType::AmneziaFreeV2:
    case serverConfigUtils::ConfigType::SelfHostedAdmin:
    case serverConfigUtils::ConfigType::SelfHostedUser:
    case serverConfigUtils::ConfigType::Invalid:
    default:
        return {};
    }
}

DockerContainer ServersController::getDefaultContainer(const QString &serverId) const
{
    switch (m_serversRepository->serverKind(serverId)) {
    case serverConfigUtils::ConfigType::Native: {
        const auto cfg = m_serversRepository->nativeConfig(serverId);
        return cfg.has_value() ? cfg->defaultContainer : DockerContainer::None;
    }
    case serverConfigUtils::ConfigType::AmneziaPremiumV2:
    case serverConfigUtils::ConfigType::AmneziaFreeV3:
    case serverConfigUtils::ConfigType::ExternalPremium:
    case serverConfigUtils::ConfigType::AmneziaPremiumV1:
    case serverConfigUtils::ConfigType::AmneziaFreeV2:
    case serverConfigUtils::ConfigType::SelfHostedAdmin:
    case serverConfigUtils::ConfigType::SelfHostedUser:
    case serverConfigUtils::ConfigType::Invalid:
    default:
        return DockerContainer::None;
    }
}

ContainerConfig ServersController::getContainerConfig(const QString &serverId, DockerContainer container) const
{
    return getServerContainersMap(serverId).value(container);
}

int ServersController::getDefaultServerIndex() const
{
    return m_serversRepository->defaultServerIndex();
}

QString ServersController::getDefaultServerId() const
{
    return m_serversRepository->defaultServerId();
}

int ServersController::getServersCount() const
{
    return m_serversRepository->serversCount();
}

QString ServersController::getServerId(int serverIndex) const
{
    return m_serversRepository->serverIdAt(serverIndex);
}

int ServersController::indexOfServerId(const QString &serverId) const
{
    return m_serversRepository->indexOfServerId(serverId);
}

QString ServersController::notificationDisplayName(const QString &serverId) const
{
    if (serverId.isEmpty()) {
        return {};
    }

    using Kind = serverConfigUtils::ConfigType;
    switch (m_serversRepository->serverKind(serverId)) {
    case Kind::Native: {
        if (const auto cfg = m_serversRepository->nativeConfig(serverId)) {
            if (!cfg->displayName.isEmpty()) {
                return cfg->displayName;
            }
        }
        break;
    }
    case Kind::AmneziaPremiumV2:
    case Kind::AmneziaFreeV3:
    case Kind::ExternalPremium:
    case Kind::AmneziaPremiumV1:
    case Kind::AmneziaFreeV2:
    case Kind::SelfHostedAdmin:
    case Kind::SelfHostedUser:
    case Kind::Invalid:
    default:
        break;
    }

    const int idx = indexOfServerId(serverId);
    if (idx >= 0) {
        return QString::number(idx + 1);
    }
    return serverId;
}

bool ServersController::hasInstalledContainers(const QString &serverId) const
{
    const QMap<DockerContainer, ContainerConfig> containers = getServerContainersMap(serverId);

    for (auto it = containers.begin(); it != containers.end(); ++it) {
        DockerContainer container = it.key();
        if (ContainerUtils::containerService(container) == ServiceType::Vpn) {
            return true;
        }
        if (container == DockerContainer::SSXray) {
            return true;
        }
    }
    return false;
}

bool ServersController::isLegacyApiV1Server(const QString &serverId) const
{
    return !serverId.isEmpty()
            && serverConfigUtils::isLegacyApiSubscription(m_serversRepository->serverKind(serverId));
}
