#ifndef COMMONSTRUCTS_H
#define COMMONSTRUCTS_H

#include <QString>
#include "core/utils/routeModes.h"

namespace amnezia
{
    struct InstalledAppInfo {
        QString appName;
        QString packageName;
        QString appPath;

        bool operator==(const InstalledAppInfo& other) const {
            if (!packageName.isEmpty()) {
                return packageName == other.packageName;
            } else {
                return appPath == other.appPath;
            }
        }
    };

    struct DnsSettings
    {
        QString primaryDns;
        QString secondaryDns;
    };

    struct SplitTunnelingSettings
    {
        bool isSitesSplitTunnelingEnabled;
        RouteMode routeMode;
    };

    struct ConnectionSettings
    {
        DnsSettings dns;
        SplitTunnelingSettings splitTunneling;
    };
}

#endif // COMMONSTRUCTS_H


