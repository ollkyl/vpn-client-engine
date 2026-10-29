#include "configuratorBase.h"

#include "core/configurators/openVpnConfigurator.h"
#include "core/configurators/xrayConfigurator.h"

using namespace amnezia;

std::unique_ptr<ConfiguratorBase> ConfiguratorBase::create(Proto protocol)
{
    switch (protocol) {
    case Proto::OpenVpn: return std::make_unique<OpenVpnConfigurator>();
    case Proto::Xray:
    case Proto::SSXray: return std::make_unique<XrayConfigurator>();
    case Proto::WireGuard:
    case Proto::Awg:
    case Proto::Ikev2: return std::make_unique<ConfiguratorBase>();
    default: return nullptr;
    }
}

ProtocolConfig ConfiguratorBase::processConfigWithLocalSettings(const ConnectionSettings &settings,
                                                                 ProtocolConfig protocolConfig)
{
    applyDnsToNativeConfig(settings.dns, protocolConfig);
    return protocolConfig;
}

void ConfiguratorBase::applyDnsToNativeConfig(const DnsSettings &dns, ProtocolConfig &protocolConfig)
{
    QString config = protocolConfig.nativeConfig();
    config.replace("$PRIMARY_DNS", dns.primaryDns);
    config.replace("$SECONDARY_DNS", dns.secondaryDns);
    protocolConfig.setNativeConfig(config);
}
