#ifndef VPNENGINE_H
#define VPNENGINE_H

#include <QObject>
#include <QScopedPointer>
#include <QThread>

#include "core/protocols/vpnProtocol.h"
#include "core/utils/errorCodes.h"
#include "core/controllers/allowedDnsController.h"
#include "core/controllers/appSplitTunnelingController.h"
#include "core/controllers/connectionController.h"
#include "core/controllers/importController.h"
#include "core/controllers/ipSplitTunnelingController.h"
#include "core/controllers/serversController.h"
#include "core/controllers/settingsController.h"

class SecureQSettings;
class SecureServersRepository;
class SecureAppSettingsRepository;
class VpnConnection;

// Entry point of the engine: owns the storage, the VPN connection thread and all core controllers.
// Create exactly one instance after Q(Core)Application. The privileged service must be installed and running
// for connections to work.
class VpnEngine : public QObject
{
    Q_OBJECT

public:
    explicit VpnEngine(QObject *parent = nullptr);
    ~VpnEngine() override;

    // Imports a config file content or a key (vpn://, vless://, ...). On success serverId receives the new server id.
    ErrorCode importConfig(const QString &data, QString *serverId = nullptr);

    // Connects to the given server, or to the default server when serverId is empty. The result is reported
    // asynchronously via connectionStateChanged() / connectionErrorOccurred().
    ErrorCode connectToServer(const QString &serverId = QString());
    void disconnectFromServer();

    Vpn::ConnectionState connectionState() const;
    bool isConnected() const;

    ServersController *servers() const { return m_serversController; }
    ImportController *importer() const { return m_importController; }
    SettingsController *settings() const { return m_settingsController; }
    IpSplitTunnelingController *siteSplitTunneling() const { return m_ipSplitTunnelingController; }
    AppSplitTunnelingController *appSplitTunneling() const { return m_appSplitTunnelingController.get(); }
    AllowedDnsController *allowedDns() const { return m_allowedDnsController.get(); }
    ConnectionController *connection() const { return m_connectionController; }

signals:
    void connectionStateChanged(Vpn::ConnectionState state);
    void connectionErrorOccurred(ErrorCode errorCode);
    void bytesChanged(quint64 receivedBytes, quint64 sentBytes);

private:
    void initLogging();
    void connectSignals();
    void onConnectionStateChanged(Vpn::ConnectionState state);

    SecureQSettings *m_settings;
    SecureServersRepository *m_serversRepository;
    SecureAppSettingsRepository *m_appSettingsRepository;

    QThread m_vpnConnectionThread;
    QScopedPointer<VpnConnection> m_vpnConnection;

    ServersController *m_serversController;
    ImportController *m_importController;
    ConnectionController *m_connectionController;
    SettingsController *m_settingsController;
    IpSplitTunnelingController *m_ipSplitTunnelingController;
    QScopedPointer<AppSplitTunnelingController> m_appSplitTunnelingController;
    QScopedPointer<AllowedDnsController> m_allowedDnsController;

    Vpn::ConnectionState m_connectionState = Vpn::ConnectionState::Disconnected;
};

#endif // VPNENGINE_H
