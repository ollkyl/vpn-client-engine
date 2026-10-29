#ifndef IMPORTCONTROLLER_H
#define IMPORTCONTROLLER_H

#include <QObject>
#include <QJsonObject>

#include "core/repositories/secureServersRepository.h"
#include "core/utils/errorCodes.h"

using namespace amnezia;

class ImportController : public QObject
{
    Q_OBJECT

public:
    enum class ConfigType {
        Amnezia,
        OpenVpn,
        WireGuard,
        Awg,
        Xray,
        ShadowSocks,
        Invalid
    };

    struct ImportResult
    {
        ErrorCode errorCode = ErrorCode::NoError;
        QJsonObject config;
        ConfigType configType = ConfigType::Invalid;
        // Non-empty for OpenVPN configs: they may run scripts, the user should confirm the import
        QString maliciousWarningText;
    };

    explicit ImportController(SecureServersRepository* serversRepository, QObject *parent = nullptr);

    // Parses a config file content or a key (vpn://, vless://, vmess://, trojan://, ss://, ssd://)
    ImportResult extractConfigFromData(const QString &data) const;

    // Stores a config returned by extractConfigFromData(). Returns the new server id, or an empty string on failure.
    QString importConfig(const QJsonObject &config);

    // Turns a plain WireGuard config into AmneziaWG with default obfuscation parameters
    static QJsonObject processNativeWireGuardConfig(const QJsonObject &config);

private:
    QJsonObject extractOpenVpnConfig(const QString &data) const;
    QJsonObject extractWireGuardConfig(const QString &data, ConfigType &configType) const;
    QJsonObject extractXrayConfig(const QString &data, ConfigType configType, const QString &description = "") const;
    static QString checkForMaliciousStrings(const QJsonObject &serverConfig);
    static void processAmneziaConfig(QJsonObject &config);

    SecureServersRepository* m_serversRepository;
};

#endif // IMPORTCONTROLLER_H
