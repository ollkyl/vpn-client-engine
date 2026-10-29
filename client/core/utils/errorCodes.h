#ifndef ERRORCODES_H
#define ERRORCODES_H

#include <QMetaEnum>
#include <QObject>

namespace amnezia
{
    namespace error_code_ns
    {
      Q_NAMESPACE
      // Numeric values match upstream Amnezia Client
      enum ErrorCode {
        // General error codes
        NoError = 0,
        InternalError = 101,
        AmneziaServiceNotRunning = 103,
        NotSupportedOnThisPlatform = 104,

        // Local errors
        OpenVpnConfigMissing = 500,
        OpenVpnManagementServerError = 501,

        // Distro errors
        AmneziaServiceConnectionFailed = 603,

        // VPN errors
        OpenVpnAdaptersInUseError = 700,
        OpenVpnUnknownError = 701,
        OpenVpnTapAdapterError = 702,

        // 3rd party utils errors
        XrayExecutableCrashed = 803,
        Tun2SockExecutableCrashed = 804,

        // Import errors
        ImportInvalidConfigError = 900,
        NoInstalledContainersError = 902,
        LegacyContainerNotSupportedError = 906,
      };
      Q_ENUM_NS(ErrorCode)
    }

    using ErrorCode = error_code_ns::ErrorCode;
}

Q_DECLARE_METATYPE(amnezia::ErrorCode)

#endif // ERRORCODES_H
