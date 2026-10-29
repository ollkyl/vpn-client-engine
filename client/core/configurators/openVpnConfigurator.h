#ifndef OPENVPN_CONFIGURATOR_H
#define OPENVPN_CONFIGURATOR_H

#include "configuratorBase.h"

class OpenVpnConfigurator : public ConfiguratorBase
{
public:
    amnezia::ProtocolConfig processConfigWithLocalSettings(const amnezia::ConnectionSettings &settings,
                                                           amnezia::ProtocolConfig protocolConfig) override;
};

#endif // OPENVPN_CONFIGURATOR_H
