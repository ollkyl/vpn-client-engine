# VPN Client Engine

**Open-source Windows VPN engine based on [Amnezia Client](https://github.com/amnezia-vpn/amnezia-client).**

A stripped-down fork of Amnezia Client designed to be reused as the VPN core of a custom VPN application.

It keeps the underlying VPN functionality — **AmneziaWG, WireGuard, XRay/VLESS, OpenVPN, IKEv2, killswitch, split tunneling, DNS and the privileged VPN service** — while removing Amnezia's product layer such as accounts, subscriptions, server marketplace, SSH deployment and server management.

Build your own VPN product on top of the engine with your own **UI, backend, authentication, subscriptions and server infrastructure**.

[![License](https://img.shields.io/badge/license-GPLv3-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows-blue)]()
[![Protocols](https://img.shields.io/badge/protocols-AmneziaWG%20%7C%20WireGuard%20%7C%20XRay%2FVLESS%20%7C%20OpenVPN%20%7C%20IKEv2-green)]()

> **Primary target: Windows.** The project is based on the cross-platform Amnezia Client codebase, but Windows is the main target for this engine.

---

## Why this exists

Amnezia Client is a complete VPN product. This project extracts the reusable client-side VPN functionality from that product and removes features that are not required when building a custom VPN application.

The goal is simple:

```text
Your application
       │
       ▼
Your UI / Backend / Authentication / Subscription
       │
       ▼
VPN Client Engine
       │
       ├── AmneziaWG
       ├── WireGuard
       ├── XRay / VLESS
       ├── OpenVPN
       └── IKEv2
```

Instead of coupling your application to Amnezia's accounts, subscriptions, marketplace or server management.

## Who is this for?

This project is intended for developers who want to build their own VPN application while reusing the VPN functionality of Amnezia Client.

Typical use cases:

- custom Windows VPN clients
- commercial VPN applications
- private VPN services
- VPN clients with custom UI and backend
- applications that need AmneziaWG or XRay/VLESS support
- projects that need killswitch and split tunneling without Amnezia's product layer

## Supported protocols

- **AmneziaWG**
- **WireGuard**
- **XRay / VLESS**
- **OpenVPN**
- **IKEv2**

## What is included

- VPN connection management
- AmneziaWG
- WireGuard
- XRay / VLESS
- OpenVPN
- IKEv2
- killswitch
- application split tunneling
- DNS configuration
- privileged VPN service
- configuration import
- logging
- connection state management

## What was removed

The following Amnezia product features are intentionally not part of this project:

- Amnezia accounts
- authentication
- subscriptions and billing
- server marketplace
- Amnezia cloud
- SSH server deployment
- server provisioning
- server-side container management
- Amnezia news and notifications
- premium/advertising UI
- Amnezia-specific multi-server management

VPN servers and configurations are supplied by the application using the engine.

## Architecture

```text
Your VPN Product
│
├── Your UI
├── Your Backend API
├── Your Authentication
├── Your Subscription System
└── Your Server Infrastructure
          │
          ▼
   VPN Client Engine
          │
          ├── WireGuard
          ├── AmneziaWG
          ├── OpenVPN
          ├── IKEv2
          └── XRay / VLESS
          │
          ├── Killswitch
          ├── Split Tunneling
          ├── DNS
          └── Privileged Service
```

## Servers and configurations

This project does not deploy or manage VPN servers.

The application using this engine is responsible for:

- obtaining VPN server configurations
- selecting a server
- storing and updating configurations
- handling user accounts and subscriptions
- managing the backend API

The engine receives a VPN configuration and handles the client-side connection.

## Building

### Requirements

- Windows 10/11
- Visual Studio 2022
- CMake
- Conan 2
- Python 3.12+
- Qt 6.10+
- Qt 5 Compatibility Module
- Qt Remote Objects

### Build

Clone the repository:

```bash
git clone https://github.com/ollkyl/vpn-client-engine.git
cd vpn-client-engine
```

Configure dependencies with Conan and build the project using CMake.

The exact build configuration may change as the project evolves.

## Running

After building the project, the Windows service and client executable can be found under the generated build directory.

Example:

```text
deploy\build\service\server\Release\AmneziaVPN-service.exe
deploy\build\client\Release\AmneziaVPN.exe
```

The privileged service is responsible for operations that require elevated Windows permissions.

## Testing

The engine has been tested by building the Windows client, importing a VPN configuration and establishing a real VPN connection.

Protocol-specific testing and additional automated tests are being expanded as the project develops.

## Custom product integration

The intended architecture is to use this repository as the VPN layer underneath your own application.

For example:

```text
Your Desktop Application
│
├── Custom UI
├── User Authentication
├── Subscription / Billing
├── Backend API
├── Server Selection
│
└── VPN Engine
     ├── AmneziaWG
     ├── WireGuard
     ├── XRay / VLESS
     ├── OpenVPN
     └── IKEv2
```

Your application can provide the configuration to the engine without using Amnezia's account or server-management infrastructure.

### Adding custom controllers

The project follows the existing Amnezia Client controller architecture.

Core controllers are located under:

```text
client/core/controllers/
```

UI controllers are located under:

```text
client/ui/controllers/
```

For example:

```text
client/core/controllers/updateController.h
client/core/controllers/updateController.cpp

client/ui/controllers/updateUiController.h
client/ui/controllers/updateUiController.cpp
```

Core controllers can expose asynchronous operations using Qt futures:

```cpp
QFuture<QPair<ErrorCode, T>>
```

and handle results with:

```cpp
.then(this, ...)
```

### Adding custom QML pages

The main QML UI is located under:

```text
client/ui/qml/
```

Pages can be added under:

```text
client/ui/qml/Pages2/
```

When adding new QML files, make sure they are included in:

```text
client/ui/qml/qml.qrc
```

If the page needs a new navigation entry, update the corresponding page enumeration and navigation logic, for example:

```text
client/ui/utils/pageEnum.h
```

The existing UI can therefore be replaced or extended while keeping the underlying VPN functionality.

## Architecture overview

The project is based on the Amnezia Client architecture and keeps the components required for client-side VPN operation.

```text
Qt/QML UI
    │
    ▼
UI Controllers
    │
    ▼
Core Controllers
    │
    ├── Configuration
    ├── Connection Management
    ├── DNS
    ├── Killswitch
    └── Split Tunneling
    │
    ▼
Privileged Windows Service
    │
    ├── WireGuard / AmneziaWG
    ├── OpenVPN
    ├── IKEv2
    └── XRay / VLESS
```

The exact internal architecture follows the upstream Amnezia Client codebase and may change as the project is refactored.

## Relationship to Amnezia Client

This project is a stripped-down derivative of [Amnezia Client](https://github.com/amnezia-vpn/amnezia-client).

The original Amnezia Client provides a complete VPN product, including UI, accounts, server management, deployment tools and other product-specific functionality.

This repository focuses on the reusable VPN client functionality needed to build a separate VPN product.

It is **not affiliated with, sponsored by, or endorsed by Amnezia**.

## Differences from AmneziaWG Windows

[AmneziaWG Windows](https://github.com/amnezia-vpn/amneziawg-windows) is a lower-level AmneziaWG/WireGuard implementation.

This project is different: it provides a higher-level multi-protocol VPN client based on Amnezia Client and includes:

- AmneziaWG
- WireGuard
- XRay / VLESS
- OpenVPN
- IKEv2
- killswitch
- split tunneling
- DNS
- privileged service
- configuration and connection management

Use a low-level AmneziaWG implementation if you only need the tunnel engine.

Use this project if you need a reusable **multi-protocol VPN client engine** for a custom VPN application.

## Project status

The project is under active development.

The main goal is to keep the VPN functionality stable while progressively removing remaining Amnezia-specific product dependencies and improving the engine's integration points for custom applications.

## License

This project is licensed under **GPLv3**, inherited from the upstream Amnezia Client project.

See [LICENSE](LICENSE) for details.

Additional third-party components may have their own licenses. Check the corresponding source directories and license files before distributing a product based on this project.

## Disclaimer

This project is an independent open-source derivative based on Amnezia Client.

It is **not affiliated with, sponsored by, or endorsed by Amnezia**.

## Keywords

`VPN` · `Windows VPN` · `VPN engine` · `VPN client` · `Amnezia Client` · `AmneziaWG` · `WireGuard` · `XRay` · `VLESS` · `OpenVPN` · `IKEv2` · `split tunneling` · `killswitch`
