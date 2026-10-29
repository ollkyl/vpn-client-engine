#ifndef SERVERSCONTROLLER_H
#define SERVERSCONTROLLER_H

#include <QObject>
#include <QVector>
#include <QMap>

#include "core/utils/containerEnum.h"
#include "core/repositories/secureServersRepository.h"
#include "core/models/containerConfig.h"
#include "core/models/serverDescription.h"

using namespace amnezia;

class ServersController : public QObject
{
    Q_OBJECT

public:
    explicit ServersController(SecureServersRepository* serversRepository, QObject *parent = nullptr);

    bool renameServer(const QString &serverId, const QString &name);
    void removeServer(const QString &serverId);
    void setDefaultServer(const QString &serverId);
    void setDefaultContainer(const QString &serverId, DockerContainer container);

    QVector<ServerDescription> serverDescriptions() const;
    int getDefaultServerIndex() const;
    QString getDefaultServerId() const;
    int getServersCount() const;
    QString getServerId(int serverIndex) const;
    int indexOfServerId(const QString &serverId) const;
    QString displayName(const QString &serverId) const;
    QMap<DockerContainer, ContainerConfig> getServerContainersMap(const QString &serverId) const;
    DockerContainer getDefaultContainer(const QString &serverId) const;
    ContainerConfig getContainerConfig(const QString &serverId, DockerContainer container) const;

    bool hasInstalledContainers(const QString &serverId) const;

private:
    void ensureDefaultServerValid();

    SecureServersRepository* m_serversRepository;
};

#endif // SERVERSCONTROLLER_H
