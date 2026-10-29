#ifndef CONFIGURATORBASE_H
#define CONFIGURATORBASE_H

#include <memory>

#include "core/utils/protocolEnum.h"
#include "core/utils/commonStructs.h"
#include "core/models/protocolConfig.h"

// Applies local connection settings (DNS, split tunneling) to a stored protocol config
// right before it is handed to the VPN protocol implementation.
class ConfiguratorBase
{
public:
    virtual ~ConfiguratorBase() = default;

    static std::unique_ptr<ConfiguratorBase> create(amnezia::Proto protocol);

    virtual amnezia::ProtocolConfig processConfigWithLocalSettings(const amnezia::ConnectionSettings &settings,
                                                                   amnezia::ProtocolConfig protocolConfig);

protected:
    static void applyDnsToNativeConfig(const amnezia::DnsSettings &dns, amnezia::ProtocolConfig &protocolConfig);
};

#endif // CONFIGURATORBASE_H
