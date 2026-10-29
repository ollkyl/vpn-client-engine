#ifndef XRAY_CONFIGURATOR_H
#define XRAY_CONFIGURATOR_H

#include "configuratorBase.h"

class XrayConfigurator : public ConfiguratorBase
{
public:
    amnezia::ProtocolConfig processConfigWithLocalSettings(const amnezia::ConnectionSettings &settings,
                                                           amnezia::ProtocolConfig protocolConfig) override;
};

#endif // XRAY_CONFIGURATOR_H
