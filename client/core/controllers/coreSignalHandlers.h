#ifndef CORESIGNALHANDLERS_H
#define CORESIGNALHANDLERS_H

#include <QObject>
#include "core/controllers/coreController.h"

class CoreSignalHandlers : public QObject
{
    Q_OBJECT

public:
    explicit CoreSignalHandlers(CoreController* coreController, QObject* parent = nullptr);

    void initAllHandlers();

private:
    void initErrorMessagesHandler();
    void initSettingsSplitTunnelingHandler();
    void initImportControllerHandler();
    void initTranslationsUpdatedHandler();
    void initLanguageHandler();
    void initAutoConnectHandler();
    void initAmneziaDnsToggledHandler();
    void initServersModelUpdateHandler();
    void initSitesModelUpdateHandler();
    void initAllowedDnsModelUpdateHandler();
    void initAppSplitTunnelingModelUpdateHandler();
    void initPrepareConfigHandler();
    void initUnsupportedConnectDrawerHandler();
    void initStrictKillSwitchHandler();
    void initAndroidSettingsHandler();
    void initAndroidConnectionHandler();
    void initIosImportHandler();
    void initIosSettingsHandler();
    void initNotificationHandler();
    void initUpdateFoundHandler();

    CoreController* m_coreController;
};

#endif // CORESIGNALHANDLERS_H

