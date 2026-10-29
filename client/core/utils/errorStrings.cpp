#include "errorStrings.h"

using namespace amnezia;

QString errorString(ErrorCode code) {
    QString errorMessage;

    switch (code) {
    case(ErrorCode::NoError): errorMessage = QObject::tr("No error"); break;
    case(ErrorCode::AmneziaServiceNotRunning): errorMessage = QObject::tr("Background service is not running"); break;
    case(ErrorCode::NotSupportedOnThisPlatform): errorMessage = QObject::tr("The selected protocol is not supported on the current platform"); break;

    case (ErrorCode::OpenVpnConfigMissing): errorMessage = QObject::tr("OpenVPN config missing"); break;
    case (ErrorCode::OpenVpnManagementServerError): errorMessage = QObject::tr("OpenVPN management server error"); break;

    case (ErrorCode::AmneziaServiceConnectionFailed): errorMessage = QObject::tr("Background service error"); break;

    case (ErrorCode::OpenVpnAdaptersInUseError): errorMessage = QObject::tr("Can't connect: another VPN connection is active"); break;
    case (ErrorCode::OpenVpnUnknownError): errorMessage = QObject::tr("OpenVPN error"); break;
    case (ErrorCode::OpenVpnTapAdapterError): errorMessage = QObject::tr("Can't setup OpenVPN TAP network adapter"); break;

    case (ErrorCode::XrayExecutableCrashed): errorMessage = QObject::tr("XRay process crashed"); break;
    case (ErrorCode::Tun2SockExecutableCrashed): errorMessage = QObject::tr("tun2socks process crashed"); break;

    case (ErrorCode::ImportInvalidConfigError): errorMessage = QObject::tr("The config is invalid or not supported"); break;
    case (ErrorCode::NoInstalledContainersError): errorMessage = QObject::tr("The server has no VPN protocols"); break;
    case (ErrorCode::LegacyContainerNotSupportedError): errorMessage = QObject::tr("This protocol is no longer supported"); break;

    case(ErrorCode::InternalError):
    default:
        errorMessage = QObject::tr("Internal error"); break;
    }

    return QObject::tr("ErrorCode: %1. ").arg(code) + errorMessage;
}

QDebug operator<<(QDebug debug, const ErrorCode &e)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "ErrorCode::" << int(e) << "(" << errorString(e) << ")";

    return debug;
}
