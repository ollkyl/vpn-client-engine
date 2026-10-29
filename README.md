# VPN Client Engine

**A Windows VPN engine without UI, extracted from [Amnezia Client](https://github.com/amnezia-vpn/amnezia-client).**

Fork it and build your own VPN application on top: your UI, your backend, your accounts and subscriptions. The engine only does the client-side VPN work.

[![License](https://img.shields.io/badge/license-GPLv3-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows-blue)]()

## What it does

- connects using a client config: **AmneziaWG, WireGuard, XRay (VLESS, VMess, Trojan, Shadowsocks), OpenVPN, IKEv2**
- imports configs: `.conf` / `.ovpn` / XRay JSON files, `vpn://` keys, `vless://`, `vmess://`, `trojan://`, `ss://`, `ssd://` links
- stores servers and settings encrypted (OS keychain)
- kill switch and strict kill switch
- split tunneling by sites/IPs and by applications
- custom DNS, allowed DNS servers
- the privileged Windows service that creates tunnels, routes and firewall rules

## What it does not do

There is no UI, no update checker, no accounts, no subscriptions, no server deployment over SSH, and no connection to Amnezia servers. Your application supplies the configs.

## Repository layout

```text
client/
  vpnEngine.h/.cpp        engine entry point (start here)
  vpnConnection.h/.cpp    VPN connection, runs in its own thread
  core/
    controllers/          servers, import, connection, settings, split tunneling, DNS
    repositories/         encrypted storage of servers and settings
    models/               server / protocol config models
    configurators/        applies local settings to a config before connecting
    protocols/            AmneziaWG/WireGuard, XRay, OpenVPN, IKEv2 implementations
    utils/
  cli/                    vpn-engine-cli: console client and usage example
  daemon/, mozilla/,
  platforms/windows/      code shared with the service (tunnel, firewall, split tunnel driver)
service/                  privileged Windows service
ipc/                      Qt Remote Objects interface between the client and the service
common/                   logger, crypto helpers
recipes/                  Conan recipes for VPN backends and drivers
deploy/                   build script, service install scripts
```

## Building

Requirements: Windows 10/11, Visual Studio 2022, CMake 3.25+, Conan 2, Python 3.12+, Qt 6.10+ (Core, Network, Widgets, Remote Objects, Core5Compat, Concurrent).

```bat
git clone --recursive https://github.com/ollkyl/vpn-client-engine.git
cd vpn-client-engine
deploy\build.bat
```

The script sets up MSVC, configures with Conan, builds, and installs a ready-to-run folder to `deploy\build\install`. VPN backends (amneziawg, openvpn, tun2socks, wintun, TAP and split-tunnel drivers) are downloaded by Conan.

CMake targets:

| Target | What |
|---|---|
| `vpn_engine` | static library, link your application to it |
| `vpn-engine-cli` | console client |
| `VpnEngine-service` | privileged service (name follows `CLIENT_SERVICE_NAME`) |

## Running

From `deploy\build\install`, as Administrator:

```bat
install_service.cmd

vpn-engine-cli import C:\path\to\config.conf
vpn-engine-cli list
vpn-engine-cli connect          :: Ctrl+C disconnects
vpn-engine-cli remove <server id>

uninstall_service.cmd
```

## Using the engine in your application

Link `vpn_engine`, create one `VpnEngine` after `QCoreApplication` / `QApplication`:

```cpp
#include "vpnEngine.h"

VpnEngine engine;

QString serverId;
if (engine.importConfig(configText, &serverId) != ErrorCode::NoError) {
    // invalid or unsupported config
}

QObject::connect(&engine, &VpnEngine::connectionStateChanged, [](Vpn::ConnectionState state) {
    // update your UI
});
QObject::connect(&engine, &VpnEngine::connectionErrorOccurred, [](ErrorCode error) {
    // errorString(error) gives a readable message
});

engine.connectToServer(serverId);
// ...
engine.disconnectFromServer();
```

Everything else is reached through the controllers:

| Accessor | Purpose |
|---|---|
| `servers()` | list (`serverDescriptions()`), rename, remove servers, choose the default server and protocol |
| `importer()` | parse a config without saving it, warning for OpenVPN scripts |
| `settings()` | DNS, kill switch, strict kill switch, logging, reset everything |
| `siteSplitTunneling()` | split tunneling by sites / IPs |
| `appSplitTunneling()` | split tunneling by applications |
| `allowedDns()` | DNS servers allowed while the kill switch is on |

`client/cli/main.cpp` is a complete small example.

## Naming your product

Set these CMake variables (defaults are `VpnEngine`):

```bat
cmake ... -DCLIENT_APPLICATION_NAME=MyVPN -DCLIENT_SERVICE_NAME=MyVPN-service -DCLIENT_ORGANIZATION_NAME=MyCompany
```

They define the settings location, log file names, the service executable and Windows service name, the WireGuard adapter and tunnel service names, and the split-tunnel driver service name. Use your own names so the engine never conflicts with another VPN client installed on the same machine.

## Compatibility with Amnezia Client

- Servers are stored in the same JSON format as in Amnezia Client.
- Only imported client configs are supported. Configs issued by the Amnezia subscription API are rejected on import.
- Internal identifiers kept from upstream: container names (`amnezia-awg`, `amnezia-xray`, ...), error code values, the `AmneziaWGTunnel$<name>` tunnel service prefix used by AmneziaWG for Windows.

## License

GPLv3, inherited from Amnezia Client. See [LICENSE](LICENSE) and [THIRD_PARTY_LICENSES.md](THIRD_PARTY_LICENSES.md).

This is an independent derivative of Amnezia Client, **not affiliated with, sponsored by, or endorsed by Amnezia**.
