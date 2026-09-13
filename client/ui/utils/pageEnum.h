#ifndef PAGE_ENUM_H
#define PAGE_ENUM_H

#include <QObject>
#include <QQmlEngine>

namespace PageLoader
{
    Q_NAMESPACE
    enum class PageEnum {
        PageStart = 0,
        PageHome,
        PageAbout,

        PageSettings,
        PageSettingsConnection,
        PageSettingsDns,
        PageSettingsApplication,
        PageSettingsBackup,
        PageSettingsAbout,
        PageSettingsLogging,
        PageSettingsSplitTunneling,
        PageSettingsAppSplitTunneling,
        PageSettingsKillSwitch,
        PageSettingsKillSwitchExceptions,

        PageDevMenu,

        PageSettingsLanguage,

        PageUpdate,
    };
    Q_ENUM_NS(PageEnum)

    static void declareQmlPageEnum()
    {
        qmlRegisterUncreatableMetaObject(PageLoader::staticMetaObject, "PageEnum", 1, 0, "PageEnum", "Error: only enums");
    }
}

#endif // PAGE_ENUM_H
