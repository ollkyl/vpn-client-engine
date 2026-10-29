#include "serversController.h"

#include "core/utils/containers/containerUtils.h"

ServersController::ServersController(SecureServersRepository *serversRepository, QObject *parent)
    : QObject(parent), m_serversRepository(serversRepository)
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
    auto cfg = m_serversRepository->serverConfig(serverId);
    if (!cfg.has_value()) {
        return false;
    }
    cfg->description = name;
    cfg->displayName = name;
    return m_serversRepository->editServer(serverId, cfg->toJson());
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
    auto cfg = m_serversRepository->serverConfig(serverId);
    if (!cfg.has_value() || !cfg->containers.contains(container)) {
        return;
    }
    cfg->defaultContainer = container;
    m_serversRepository->editServer(serverId, cfg->toJson());
}

QVector<ServerDescription> ServersController::serverDescriptions() const
{
    QVector<ServerDescription> out;
    const QVector<QString> ids = m_serversRepository->orderedServerIds();
    out.reserve(ids.size());

    for (const QString &id : ids) {
        if (const auto cfg = m_serversRepository->serverConfig(id)) {
            out.append(buildServerDescription(id, *cfg));
        }
    }
    return out;
}

QMap<DockerContainer, ContainerConfig> ServersController::getServerContainersMap(const QString &serverId) const
{
    const auto cfg = m_serversRepository->serverConfig(serverId);
    return cfg.has_value() ? cfg->containers : QMap<DockerContainer, ContainerConfig> {};
}

DockerContainer ServersController::getDefaultContainer(const QString &serverId) const
{
    const auto cfg = m_serversRepository->serverConfig(serverId);
    return cfg.has_value() ? cfg->defaultContainer : DockerContainer::None;
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

QString ServersController::displayName(const QString &serverId) const
{
    if (const auto cfg = m_serversRepository->serverConfig(serverId); cfg && !cfg->displayName.isEmpty()) {
        return cfg->displayName;
    }

    const int idx = indexOfServerId(serverId);
    return idx >= 0 ? QString::number(idx + 1) : serverId;
}

bool ServersController::hasInstalledContainers(const QString &serverId) const
{
    const QMap<DockerContainer, ContainerConfig> containers = getServerContainersMap(serverId);
    for (auto it = containers.cbegin(); it != containers.cend(); ++it) {
        if (ContainerUtils::containerService(it.key()) == ServiceType::Vpn || it.key() == DockerContainer::SSXray) {
            return true;
        }
    }
    return false;
}
