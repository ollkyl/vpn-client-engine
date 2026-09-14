# VPN Client Engine

**Reusable Windows VPN engine based on [Amnezia Client](https://github.com/amnezia-vpn/amnezia-client).**

A stripped-down, reusable VPN client core for developers who want to build their **own VPN application with an independent frontend and backend**.

This project preserves the VPN functionality and Windows networking architecture of Amnezia Client while removing Amnezia-specific product functionality such as accounts, subscriptions, server marketplace, SSH-based server deployment, and product-specific UI.

[![License](https://img.shields.io/badge/license-GPLv3-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-blue.svg)]()
[![Protocols](https://img.shields.io/badge/protocols-OpenVPN%20%7C%20WireGuard%20%7C%20AmneziaWG%20%7C%20XRay%20%7C%20IKEv2-green.svg)]()

---

## What is this?

`vpn-client-engine` is a **stripped-down fork of Amnezia Client intended to be used as a reusable VPN engine** for custom VPN applications.

The project is designed for a common architecture:

```text
Your VPN Product
│
├── Your Frontend
├── Your Backend / API
├── Your Authentication
├── Your Subscriptions
└── Your Server Provisioning
            │
            ▼
     VPN Client Engine
            │
            ├── AmneziaWG
            ├── WireGuard
            ├── XRay / VLESS
            ├── OpenVPN
            └── IKEv2
            │
            ├── Killswitch
            ├── Split Tunneling
            ├── DNS
            └── Privileged Service
```

The engine does not depend on a particular VPN backend, account system, subscription service, or server marketplace.

VPN server configurations are provided externally.

---

## Why this exists

[Amnezia Client](https://github.com/amnezia-vpn/amnezia-client) is a mature multi-platform VPN client with support for multiple VPN protocols and advanced networking functionality.

However, the upstream application is also a complete VPN product. It includes functionality such as:

- Amnezia cloud accounts
- subscriptions and billing
- server marketplace
- server management
- SSH-based server deployment
- multi-server product UI
- product-specific notifications and integrations

If you are building a different VPN product, much of this functionality is unnecessary.

This project extracts the reusable VPN client functionality from that product layer while preserving the underlying VPN implementations and privileged service architecture.

---

## Who is this for?

This project is intended for:

- developers building custom Windows VPN clients;
- teams building white-label VPN applications;
- developers who want to build their own frontend and backend around Amnezia's VPN stack;
- VPN products with their own authentication and subscription systems;
- projects that need AmneziaWG and XRay/VLESS alongside other supported protocols;
- developers looking for a starting point closer to a reusable VPN engine than the full Amnezia Client.

If you only need a low-level AmneziaWG tunnel library, this project is not intended to replace [AmneziaWG for Windows](https://github.com/amnezia-vpn/amneziawg-windows). This repository operates at a higher level and preserves the multi-protocol client/service architecture of Amnezia Client.

---

## Supported platforms

- Windows
- Linux
- macOS

The primary target for the reusable engine is **Windows**.

---

## Supported VPN protocols

- OpenVPN
- WireGuard
- AmneziaWG (awg)
- IKEv2
- XRay / VLESS

---

## What is included

The following VPN functionality remains available:

- VPN client and privileged background service
- VPN tunnel establishment
- connection management
- protocol configuration
- configuration import
- killswitch
- application and website split tunneling
- DNS configuration and handling
- routing and network handling
- core/service communication
- logging
- backup and restore
- application update checks

The underlying protocol implementations and service architecture are based on the upstream Amnezia Client project.

---

## What was removed

The project removes functionality that belongs to the Amnezia product rather than the underlying VPN client.

### Product and account layer

- Amnezia cloud accounts
- login and account management
- subscriptions and billing
- news and product notifications
- server marketplace
- product-specific advertisements and premium UI
- Amnezia-specific product integrations

### Server management

- multi-server product management
- server marketplace integration
- SSH-based server installation and deployment
- self-hosted multi-user server administration
- automatic installation and management of server-side containers

The client assumes that VPN server configurations are supplied externally.

### Removed UI and supporting code

Several C++ UI-model classes, QML pages, fields, notifications, and other components that existed only for removed product functionality have been removed.

Unused protocol model entries and unreachable `PageEnum` values were also removed where they were no longer required by the remaining client functionality.

---

## External server configuration

This project does not provide:

- a VPN server marketplace;
- a server provisioning backend;
- an account system;
- a subscription system;
- SSH-based server deployment.

A VPN configuration is supplied by the application or backend using a compatible Amnezia configuration format.

For example, a custom VPN product can use its own backend:

```text
Your Backend
     │
     │ authentication / subscription
     │ server selection
     │ VPN configuration
     ▼
Your Frontend
     │
     ▼
VPN Client Engine
     │
     ▼
VPN Server
```

Server configurations can be:

- generated by your own backend;
- provided through your own provisioning system;
- generated manually using compatible server-side tooling.

This keeps the VPN engine independent from any particular backend or server-management platform.

---

## Architecture

At a high level, the project separates the VPN functionality from the product layer:

```text
┌─────────────────────────────────────┐
│          Your VPN Product           │
│                                     │
│  Frontend                           │
│  Backend / API                      │
│  Authentication                     │
│  Subscriptions                      │
│  Server Provisioning                │
└──────────────────┬──────────────────┘
                   │
                   ▼
┌─────────────────────────────────────┐
│          VPN Client Engine          │
│                                     │
│  AmneziaWG                          │
│  WireGuard                          │
│  XRay / VLESS                       │
│  OpenVPN                            │
│  IKEv2                              │
│                                     │
│  Connection Management              │
│  Killswitch                         │
│  Split Tunneling                    │
│  DNS                                │
│  Routing                            │
└──────────────────┬──────────────────┘
                   │
                   ▼
┌─────────────────────────────────────┐
│       Privileged Platform Service   │
│                                     │
│  Windows networking                 │
│  Tunnel management                  │
│  Firewall / routing operations      │
└─────────────────────────────────────┘
```

The project is intentionally focused on the **client-side VPN layer**. Product-specific functionality can be implemented independently.

---

## Building

The build toolchain is based on the upstream Amnezia Client project.

### Requirements

- CMake
- Conan 2
- Python 3.12+
- Qt 6.10+

### Windows

- Visual Studio 2022 or Visual Studio 2022 Build Tools
- Qt 6.10+
- Qt 5 Compatibility Module
- Qt Remote Objects

### Linux

- make
- gcc
- Qt 6.10+

> Python 3.12+ is required because some Conan recipes use f-string syntax introduced by PEP 701. Older Python versions may cause Conan to fail with a `SyntaxError` before the build starts.

### Build

**Windows:**

```bash
deploy\\build.bat
```

**Linux:**

```bash
./deploy/build.sh
```

Run either script with `-h` to see the available options.

For example:

```bash
--installer all
```

can be used to build an installer in addition to the raw binaries.

---

## Running

The privileged helper service must run with elevated privileges:

- Administrator on Windows
- root on Linux

Without the required privileges, platform networking operations may fail and the VPN connection will not be established.

### Windows

Start the service from an elevated PowerShell:

```powershell
deploy\\build\\service\\server\\Release\\AmneziaVPN-service.exe x
```

Then start the client from a regular PowerShell:

```powershell
deploy\\build\\client\\Release\\AmneziaVPN.exe
```

The service executable requires a non-empty command-line argument when started manually.

Without arguments, it attempts to start through the Windows Service Control Manager and exits unless it was actually launched by the SCM.

Any non-empty argument makes it run as a regular foreground process, which is the intended mode for development and manual testing.

---

## Testing

The built client was tested after compilation to verify that the resulting binaries can establish a real VPN connection, rather than only completing the build successfully.

### Windows

1. Start the privileged service from an elevated PowerShell:

```powershell
cd deploy\\build\\service\\server\\Release
.\\AmneziaVPN-service.exe x
```

2. Start the client from a regular PowerShell:

```powershell
deploy\\build\\client\\Release\\AmneziaVPN.exe
```

3. In the opened client:

   - Go to **Home**
   - Select **Import configuration**
   - Import a valid Amnezia-compatible server configuration
   - Connect to the imported server

The client was successfully tested with an imported server configuration after building the project from source.

This verifies that the extracted client functionality is operational and capable of establishing a real VPN connection.

---

## Building a custom VPN product on top

The project intentionally leaves product-specific functionality to the application developer.

A custom product can implement its own:

- frontend
- backend API
- authentication
- user management
- subscriptions
- payments
- server selection
- VPN configuration delivery
- analytics
- product branding

The existing Qt/C++ architecture can be extended with custom controllers and QML UI when the application uses the included client directly.

### Core controller

Create a core-layer `QObject` responsible for HTTP communication and business logic.

See:

```text
client/core/controllers/updateController.h
client/core/controllers/updateController.cpp
```

for an example of the asynchronous controller pattern used by the project.

The codebase uses:

```cpp
QFuture<QPair<ErrorCode, T>>
```

with results consumed through:

```cpp
.then(this, ...)
```

### QML UI controller

Add a thin `*UiController` layer that exposes core functionality to QML through `Q_INVOKABLE` methods, public slots, and signals.

Example:

```text
client/ui/controllers/updateUiController.h
client/ui/controllers/updateUiController.cpp
```

### Register the controller

Register the controller in:

```text
client/core/controllers/coreController.cpp
```

using the existing controller initialization mechanism.

Then expose it to QML with:

```cpp
setQmlContextProperty("YourController", yourUiController);
```

### Add your own UI

Custom QML pages can be added under:

```text
client/ui/qml/Pages2/
```

The page must also be added to:

```text
client/ui/qml/qml.qrc
```

`qml.qrc` is a manually maintained resource list, so forgetting to add a new page there will prevent the build from including the new page.

New page identifiers can be added to:

```text
client/ui/utils/pageEnum.h
```

The page enum is not persisted, so adding new values is safe.

---

## Relationship to Amnezia Client

This project is derived from the open-source [Amnezia Client](https://github.com/amnezia-vpn/amnezia-client).

The upstream project provides the VPN protocol implementations, networking architecture, privileged service model, and other technical foundations from which this project is derived.

The purpose of this repository is to remove Amnezia-specific product functionality and provide a more reusable starting point for independent VPN applications.

This project is **not affiliated with, sponsored by, or endorsed by Amnezia VPN**.

---

## License

GPLv3, inherited from the upstream Amnezia Client project.

See [LICENSE](LICENSE) for the full license text.

This repository also contains third-party components with their own licenses. When redistributing or modifying the project, comply with the GPLv3 requirements and the licenses applicable to included third-party components.

---

## Keywords

`VPN engine` · `Windows VPN` · `VPN client` · `Amnezia Client fork` · `AmneziaWG` · `WireGuard` · `XRay` · `VLESS` · `OpenVPN` · `IKEv2` · `VPN core` · `custom VPN client` · `white-label VPN` · `headless VPN`

```
