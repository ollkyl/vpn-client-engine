#ifndef CORECONTROLLER_H
#define CORECONTROLLER_H

#include <QObject>
#include <QQmlContext>
#include <QThread>

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    #include "ui/utils/systemTrayNotificationHandler.h"
#endif

#include "ui/controllers/appSplitTunnelingUiController.h"
// NOTE: this is the public "engine" tree - a backend HTTP client and its UI controller were
// intentionally left out here (see README for how to plug in your own backend).
#include "ui/controllers/allowedDnsUiController.h"
#include "ui/controllers/connectionUiController.h"
#include "ui/controllers/qml/focusController.h"
#include "ui/controllers/importUiController.h"
#include "core/controllers/selfhosted/importController.h"
#include "ui/controllers/qml/pageController.h"
#include "ui/controllers/settingsUiController.h"
#include "ui/controllers/serversUiController.h"
#include "ui/controllers/ipSplitTunnelingUiController.h"
#include "ui/controllers/systemController.h"
#include "ui/controllers/languageUiController.h"
#include "ui/controllers/updateUiController.h"
#include "ui/controllers/networkReachabilityController.h"

#include "core/controllers/serversController.h"
#include "core/controllers/appSplitTunnelingController.h"
#include "core/controllers/ipSplitTunnelingController.h"
#include "core/controllers/allowedDnsController.h"
#include "core/controllers/settingsController.h"
#include "core/controllers/connectionController.h"
#include "core/controllers/updateController.h"

#include "core/repositories/secureServersRepository.h"
#include "core/repositories/secureAppSettingsRepository.h"
#include "secureQSettings.h"

#include "ui/models/allowedDnsModel.h"
#include "ui/models/containersModel.h"
#include "ui/models/languageModel.h"
#include "ui/models/appSplitTunnelingModel.h"
#include "ui/models/serversModel.h"

#include "ui/models/ipSplitTunnelingModel.h"

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    #include "ui/utils/notificationHandler.h"
#endif

class CoreSignalHandlers;

class CoreController : public QObject
{
    Q_OBJECT
    friend class CoreSignalHandlers;

public:
    explicit CoreController(const QSharedPointer<VpnConnection> &vpnConnection, SecureQSettings* settings,
                            QQmlApplicationEngine *engine, QObject *parent = nullptr,
                            bool skipPlatformControllerInit = false);

    PageController* pageController() const;
    void setQmlRoot();
    void checkForAppUpdates();

    void openConnectionByIndex(int serverIndex);
    void importConfigFromData(const QString &data);
    void updateTranslator(const QLocale &locale);

signals:
    void translationsUpdated();
    void websiteUrlChanged(const QString &newUrl);

protected:
    SecureServersRepository* serversRepositoryProtected() const { return m_serversRepository; }
    SecureAppSettingsRepository* appSettingsRepositoryProtected() const { return m_appSettingsRepository; }
    ServersModel* serversModelProtected() const { return m_serversModel; }
    ContainersModel* containersModelProtected() const { return m_containersModel; }
    AllowedDnsModel* allowedDnsModelProtected() const { return m_allowedDnsModel; }
    AppSplitTunnelingModel* appSplitTunnelingModelProtected() const { return m_appSplitTunnelingModel; }
    IpSplitTunnelingModel* ipSplitTunnelingModelProtected() const { return m_ipSplitTunnelingModel; }
    LanguageModel* languageModelProtected() const { return m_languageModel; }
    ConnectionUiController* connectionUiControllerProtected() const { return m_connectionUiController; }
    ImportController* importCoreControllerProtected() const { return m_importCoreController; }
    ServersController* serversControllerProtected() const { return m_serversController; }
    SettingsUiController* settingsUiControllerProtected() const { return m_settingsUiController; }
    SettingsController* settingsControllerProtected() const { return m_settingsController; }
    AllowedDnsUiController* allowedDnsUiControllerProtected() const { return m_allowedDnsUiController; }
    AllowedDnsController* allowedDnsControllerProtected() const { return m_allowedDnsController; }
    LanguageUiController* languageUiControllerProtected() const { return m_languageUiController; }
    IpSplitTunnelingController* ipSplitTunnelingControllerProtected() const { return m_ipSplitTunnelingController; }
    IpSplitTunnelingUiController* ipSplitTunnelingUiControllerProtected() const { return m_ipSplitTunnelingUiController; }
    AppSplitTunnelingController* appSplitTunnelingControllerProtected() const { return m_appSplitTunnelingController; }
    AppSplitTunnelingUiController* appSplitTunnelingUiControllerProtected() const { return m_appSplitTunnelingUiController; }
    ServersUiController* serversUiControllerProtected() const { return m_serversUiController; }

private:
    void initRepositories();
    void initCoreControllers();
    void initModels();
    void initControllers();
    void initAndroidController();
    void initAppleController();
    void initLogging();
    void initSignalHandlers();
    void setQmlContextProperty(const QString &name, QObject *value);

    QQmlApplicationEngine *m_engine {}; // TODO use parent child system here?
    SecureQSettings* m_settings;
    QSharedPointer<VpnConnection> m_vpnConnection;
    QTranslator* m_translator;

    SecureServersRepository* m_serversRepository;
    SecureAppSettingsRepository* m_appSettingsRepository;

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    NotificationHandler* m_notificationHandler;
#endif

    QMetaObject::Connection m_reloadConfigErrorOccurredConnection;

    ConnectionUiController* m_connectionUiController;
    FocusController* m_focusController;
    PageController* m_pageController;
    ImportUiController* m_importController;
    ImportController* m_importCoreController;
    SettingsUiController* m_settingsUiController;
    ServersUiController* m_serversUiController;
    IpSplitTunnelingUiController* m_ipSplitTunnelingUiController;
    SystemController* m_systemController;
    NetworkReachabilityController* m_networkReachabilityController;
    AppSplitTunnelingUiController* m_appSplitTunnelingUiController;
    AllowedDnsUiController* m_allowedDnsUiController;
    LanguageUiController* m_languageUiController;
    UpdateUiController* m_updateUiController;

    ServersController* m_serversController;
    AppSplitTunnelingController* m_appSplitTunnelingController;
    IpSplitTunnelingController* m_ipSplitTunnelingController;
    AllowedDnsController* m_allowedDnsController;
    UpdateController* m_updateController;
    ConnectionController* m_connectionController;
    SettingsController* m_settingsController;

    ContainersModel* m_containersModel;
    ContainersModel* m_defaultServerContainersModel;
    ServersModel* m_serversModel;
    LanguageModel* m_languageModel;
    IpSplitTunnelingModel* m_ipSplitTunnelingModel;
    AllowedDnsModel* m_allowedDnsModel;
    AppSplitTunnelingModel* m_appSplitTunnelingModel;

    CoreSignalHandlers* m_signalHandlers;
};

#endif // CORECONTROLLER_H
