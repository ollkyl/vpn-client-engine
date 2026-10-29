#ifndef SECURESERVERSREPOSITORY_H
#define SECURESERVERSREPOSITORY_H

#include <QHash>
#include <QJsonObject>
#include <QObject>
#include <QVector>
#include <optional>

#include "core/models/nativeServerConfig.h"
#include "core/models/containerConfig.h"
#include "secureQSettings.h"

using namespace amnezia;

class SecureServersRepository : public QObject
{
    Q_OBJECT

public:
    explicit SecureServersRepository(SecureQSettings *settings, QObject *parent = nullptr);

    // Returns the id of the stored server, or an empty string if serverJson is not a supported config
    QString addServer(const QString &serverId, const QJsonObject &serverJson);
    bool editServer(const QString &serverId, const QJsonObject &serverJson);
    void removeServer(const QString &serverId);

    std::optional<NativeServerConfig> serverConfig(const QString &serverId) const;

    int serversCount() const;
    int indexOfServerId(const QString &serverId) const;
    QString serverIdAt(int index) const;
    QVector<QString> orderedServerIds() const;

    int defaultServerIndex() const;
    QString defaultServerId() const;
    void setDefaultServer(const QString &serverId);

    void clearServers();

    QString nextAvailableServerName() const;

    void invalidateCache();

signals:
    void serverAdded(const QString &serverId);
    void serverEdited(const QString &serverId);
    void serverRemoved(const QString &serverId, int removedIndex);
    void defaultServerChanged(const QString &defaultServerId);

private:
    void loadFromStorage();
    void updateDefaultServerFromStorage();
    void persistDefaultServerFields();

    QString normalizedOrGeneratedServerId(const QString &candidateId) const;

    void syncToStorage();
    QVariant value(const QString &key, const QVariant &defaultValue) const;
    void setValue(const QString &key, const QVariant &value);

    void clearServerStateMaps();

    SecureQSettings *m_settings;

    QHash<QString, QJsonObject> m_serverJsonById;
    QVector<QString> m_orderedServerIds;

    QString m_defaultServerId;
};

#endif // SECURESERVERSREPOSITORY_H
