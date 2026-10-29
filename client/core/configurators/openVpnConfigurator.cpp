#include "openVpnConfigurator.h"

#include <QRegularExpression>

#include "core/utils/constants/protocolConstants.h"
#include "core/utils/routeModes.h"

using namespace amnezia;

ProtocolConfig OpenVpnConfigurator::processConfigWithLocalSettings(const ConnectionSettings &settings,
                                                                   ProtocolConfig protocolConfig)
{
    applyDnsToNativeConfig(settings.dns, protocolConfig);

    QString config = protocolConfig.nativeConfig();

    config.replace(QRegularExpression("redirect-gateway.*"), "");

    if (settings.dns.primaryDns.contains(protocols::dns::amneziaDnsIp)) {
        config.replace(QRegularExpression("dhcp-option DNS " + settings.dns.secondaryDns), "");
    }

    if (!settings.splitTunneling.isSitesSplitTunnelingEnabled) {
        config.append("\nredirect-gateway def1 ipv6 bypass-dhcp\n");
        config.append("block-ipv6\n");
    } else if (settings.splitTunneling.routeMode == RouteMode::VpnAllExceptSites) {
        config.append("\nredirect-gateway ipv6 !ipv4 bypass-dhcp\n");
        config.append("block-ipv6\n");
    }
    // VpnOnlyForwardSites: no redirect-gateway, routes are added per site

    protocolConfig.setNativeConfig(config);
    return protocolConfig;
}
