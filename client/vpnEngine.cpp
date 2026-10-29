#include "vpnEngine.h"

#include "core/repositories/secureAppSettingsRepository.h"
#include "core/repositories/secureServersRepository.h"
#include "logger.h"
#include "secureQSettings.h"
#include "version.h"
#include "vpnConnection.h"

VpnEngine::VpnEngine(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<DockerContainer>("DockerContainer");
    qRegisterMetaType<Vpn::ConnectionState>("Vpn::ConnectionState");
    qRegisterMetaType<ErrorCode>("ErrorCode");

    m_settings = new SecureQSettings(ORGANIZATION_NAME, APPLICATION_NAME, this);
    m_serversRepository = new SecureServersRepository(m_settings, this);
    m_appSettingsRepository = new SecureAppSettingsRepository(m_settings, this);

    m_vpnConnection.reset(new VpnConnection(m_appSettingsRepository));
    m_vpnConnection->moveToThread(&m_vpnConnectionThread);
    m_vpnConnectionThread.start();

    m_serversController = new ServersController(m_serversRepository, this);
    m_importController = new ImportController(m_serversRepository, this);
    m_connectionController = new ConnectionController(m_serversRepository, m_appSettingsRepository, m_vpnConnection.get(), this);
    m_settingsController = new SettingsController(m_serversRepository, m_appSettingsRepository, this);
    m_ipSplitTunnelingController = new IpSplitTunnelingController(m_appSettingsRepository, this);
    m_appSplitTunnelingController.reset(new AppSplitTunnelingController(m_appSettingsRepository));
    m_allowedDnsController.reset(new AllowedDnsController(m_appSettingsRepository));

    connectSignals();
    initLogging();
}

VpnEngine::~VpnEngine()
{
    if (m_vpnConnectionThread.isRunning()) {
        QMetaObject::invokeMethod(m_vpnConnection.get(), &VpnConnection::disconnectSlots, Qt::BlockingQueuedConnection);
        QMetaObject::invokeMethod(m_vpnConnection.get(), &VpnConnection::disconnectFromVpn, Qt::BlockingQueuedConnection);
    }

    m_vpnConnectionThread.quit();
    if (!m_vpnConnectionThread.wait(3000)) {
        m_vpnConnectionThread.terminate();
        m_vpnConnectionThread.wait(500);
    }
}

void VpnEngine::connectSignals()
{
    connect(m_connectionController, &ConnectionController::connectionStateChanged, this, &VpnEngine::onConnectionStateChanged);
    connect(m_vpnConnection.get(), &VpnConnection::bytesChanged, this, &VpnEngine::bytesChanged);

    connect(m_settingsController, &SettingsController::strictKillSwitchModeChanged, m_connectionController,
            &ConnectionController::onKillSwitchModeChanged);

    connect(m_settingsController, &SettingsController::siteSplitTunnelingRouteModeChanged, this,
            [this](RouteMode mode) { m_ipSplitTunnelingController->setRouteMode(mode); });
    connect(m_settingsController, &SettingsController::siteSplitTunnelingToggled, this,
            [this](bool enabled) { m_ipSplitTunnelingController->toggleSplitTunneling(enabled); });
    connect(m_settingsController, &SettingsController::appSplitTunnelingRouteModeChanged, this,
            [this](AppsRouteMode mode) { m_appSplitTunnelingController->setRouteMode(mode); });
    connect(m_settingsController, &SettingsController::appSplitTunnelingToggled, this,
            [this](bool enabled) { m_appSplitTunnelingController->toggleSplitTunneling(enabled); });
}

void VpnEngine::initLogging()
{
    m_settingsController->checkIfNeedDisableLogs();

    const bool enabled = m_settingsController->isLoggingEnabled();
    if (enabled && !Logger::init(false)) {
        qWarning() << "Initialization of debug subsystem failed";
    }
    Logger::setServiceLogsEnabled(enabled);
}

ErrorCode VpnEngine::importConfig(const QString &data, QString *serverId)
{
    const ImportController::ImportResult result = m_importController->extractConfigFromData(data);
    if (result.errorCode != ErrorCode::NoError) {
        return result.errorCode;
    }

    const QString id = m_importController->importConfig(result.config);
    if (id.isEmpty()) {
        return ErrorCode::ImportInvalidConfigError;
    }

    if (serverId) {
        *serverId = id;
    }
    return ErrorCode::NoError;
}

ErrorCode VpnEngine::connectToServer(const QString &serverId)
{
    if (m_connectionState != Vpn::ConnectionState::Disconnected && m_connectionState != Vpn::ConnectionState::Error) {
        return ErrorCode::NoError;
    }

    const QString id = serverId.isEmpty() ? m_serversController->getDefaultServerId() : serverId;
    if (id.isEmpty()) {
        return ErrorCode::InternalError;
    }

    m_serversController->setDefaultServer(id);
    return m_connectionController->openConnection(id);
}

void VpnEngine::disconnectFromServer()
{
    m_connectionController->closeConnection();
}

Vpn::ConnectionState VpnEngine::connectionState() const
{
    return m_connectionState;
}

bool VpnEngine::isConnected() const
{
    return m_connectionState == Vpn::ConnectionState::Connected;
}

void VpnEngine::onConnectionStateChanged(Vpn::ConnectionState state)
{
    m_connectionState = state;
    emit connectionStateChanged(state);

    if (state == Vpn::ConnectionState::Error || state == Vpn::ConnectionState::Unknown) {
        emit connectionErrorOccurred(m_connectionController->lastConnectionError());
    }
}
