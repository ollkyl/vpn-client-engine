#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>
#include <QDateTime>

#include "core/utils/routeModes.h"
#include "core/repositories/secureServersRepository.h"
#include "core/repositories/secureAppSettingsRepository.h"

using namespace amnezia;

class SettingsController : public QObject
{
    Q_OBJECT
public:
    explicit SettingsController(SecureServersRepository* serversRepository,
                               SecureAppSettingsRepository* appSettingsRepository,
                               QObject* parent = nullptr);

    void toggleAmneziaDns(bool enable);
    bool isAmneziaDnsEnabled() const;

    QString getPrimaryDns() const;
    void setPrimaryDns(const QString &dns);

    QString getSecondaryDns() const;
    void setSecondaryDns(const QString &dns);

    bool isLoggingEnabled() const;
    void toggleLogging(bool enable);
    void clearLogs();
    // Logging switches itself off two weeks after it was enabled
    void checkIfNeedDisableLogs();

    bool isKillSwitchEnabled() const;
    void toggleKillSwitch(bool enable);

    bool isStrictKillSwitchEnabled() const;
    void toggleStrictKillSwitch(bool enable);

    QString getInstallationUuid(bool createIfNotExists = true) const;

    // Removes all servers and resets every engine setting to its default
    void clearSettings();

signals:
    // Effective strict kill switch state: false whenever the kill switch itself is off
    void strictKillSwitchModeChanged(bool enabled);

    void siteSplitTunnelingRouteModeChanged(RouteMode mode);
    void siteSplitTunnelingToggled(bool enabled);
    void appSplitTunnelingRouteModeChanged(AppsRouteMode mode);
    void appSplitTunnelingToggled(bool enabled);

private:
    SecureServersRepository* m_serversRepository;
    SecureAppSettingsRepository* m_appSettingsRepository;
};

#endif
