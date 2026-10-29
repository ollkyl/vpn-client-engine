#include "settingsController.h"

#include "logger.h"

namespace
{
    constexpr int logsLifetimeDays = 14;
}

SettingsController::SettingsController(SecureServersRepository* serversRepository,
                                     SecureAppSettingsRepository* appSettingsRepository,
                                     QObject* parent)
    : QObject(parent),
      m_serversRepository(serversRepository),
      m_appSettingsRepository(appSettingsRepository)
{
}

void SettingsController::toggleAmneziaDns(bool enable)
{
    m_appSettingsRepository->setUseAmneziaDns(enable);
}

bool SettingsController::isAmneziaDnsEnabled() const
{
    return m_appSettingsRepository->useAmneziaDns();
}

QString SettingsController::getPrimaryDns() const
{
    return m_appSettingsRepository->primaryDns();
}

void SettingsController::setPrimaryDns(const QString &dns)
{
    m_appSettingsRepository->setPrimaryDns(dns);
}

QString SettingsController::getSecondaryDns() const
{
    return m_appSettingsRepository->secondaryDns();
}

void SettingsController::setSecondaryDns(const QString &dns)
{
    m_appSettingsRepository->setSecondaryDns(dns);
}

bool SettingsController::isLoggingEnabled() const
{
    return m_appSettingsRepository->isSaveLogs();
}

void SettingsController::toggleLogging(bool enable)
{
    m_appSettingsRepository->setSaveLogs(enable);
    if (enable) {
        if (!Logger::init(false)) {
            qWarning() << "Initialization of debug subsystem failed";
        }
        m_appSettingsRepository->setLogEnableDate(QDateTime::currentDateTime());
    } else {
        Logger::deInit();
    }
    Logger::setServiceLogsEnabled(enable);
}

void SettingsController::clearLogs()
{
    Logger::clearLogs(false);
    Logger::clearServiceLogs();
}

void SettingsController::checkIfNeedDisableLogs()
{
    if (!m_appSettingsRepository->isSaveLogs()) {
        return;
    }
    const QDateTime disableDate = m_appSettingsRepository->getLogEnableDate().addDays(logsLifetimeDays);
    if (disableDate <= QDateTime::currentDateTime()) {
        toggleLogging(false);
        clearLogs();
    }
}

bool SettingsController::isKillSwitchEnabled() const
{
    return m_appSettingsRepository->isKillSwitchEnabled();
}

void SettingsController::toggleKillSwitch(bool enable)
{
    m_appSettingsRepository->setKillSwitchEnabled(enable);
    emit strictKillSwitchModeChanged(enable && isStrictKillSwitchEnabled());
}

bool SettingsController::isStrictKillSwitchEnabled() const
{
    return m_appSettingsRepository->isStrictKillSwitchEnabled();
}

void SettingsController::toggleStrictKillSwitch(bool enable)
{
    m_appSettingsRepository->setStrictKillSwitchEnabled(enable);
    emit strictKillSwitchModeChanged(enable && isKillSwitchEnabled());
}

QString SettingsController::getInstallationUuid(bool createIfNotExists) const
{
    return m_appSettingsRepository->getInstallationUuid(createIfNotExists);
}

void SettingsController::clearSettings()
{
    m_appSettingsRepository->clearSettings();
    m_serversRepository->clearServers();

    emit siteSplitTunnelingRouteModeChanged(RouteMode::VpnOnlyForwardSites);
    emit siteSplitTunnelingToggled(false);

    emit appSplitTunnelingRouteModeChanged(AppsRouteMode::VpnAllExceptApps);
    emit appSplitTunnelingToggled(false);
}
