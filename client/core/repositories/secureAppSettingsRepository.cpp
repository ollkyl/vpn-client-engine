#include "secureAppSettingsRepository.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QUuid>


using namespace amnezia;

SecureAppSettingsRepository::SecureAppSettingsRepository(SecureQSettings* settings, QObject *parent)
    : QObject(parent), m_settings(settings)
{
}

QVariant SecureAppSettingsRepository::value(const QString &key, const QVariant &defaultValue) const
{
    return m_settings->value(key, defaultValue);
}

void SecureAppSettingsRepository::setValue(const QString &key, const QVariant &value)
{
    m_settings->setValue(key, value);
}

bool SecureAppSettingsRepository::useAmneziaDns() const
{
    return value("Conf/useAmneziaDns", true).toBool();
}

void SecureAppSettingsRepository::setUseAmneziaDns(bool enabled)
{
    setValue("Conf/useAmneziaDns", enabled);
    emit useAmneziaDnsChanged(enabled);
}

QStringList SecureAppSettingsRepository::getAllowedDnsServers() const
{
    return value("Conf/allowedDnsServers").toStringList();
}

void SecureAppSettingsRepository::setAllowedDnsServers(const QStringList &servers)
{
    setValue("Conf/allowedDnsServers", servers);
    emit allowedDnsServersChanged(servers);
}

QString SecureAppSettingsRepository::primaryDns() const
{
    constexpr char cloudFlareNs1[] = "1.1.1.1";
    return value("Conf/primaryDns", cloudFlareNs1).toString();
}

void SecureAppSettingsRepository::setPrimaryDns(const QString &dns)
{
    setValue("Conf/primaryDns", dns);
}

QString SecureAppSettingsRepository::secondaryDns() const
{
    constexpr char cloudFlareNs2[] = "1.0.0.1";
    return value("Conf/secondaryDns", cloudFlareNs2).toString();
}

void SecureAppSettingsRepository::setSecondaryDns(const QString &dns)
{
    setValue("Conf/secondaryDns", dns);
}

namespace {
    QString routeModeString(RouteMode mode) {
        switch (mode) {
        case RouteMode::VpnAllSites: return "AllSites";
        case RouteMode::VpnOnlyForwardSites: return "ForwardSites";
        case RouteMode::VpnAllExceptSites: return "ExceptSites";
        }
        return QString();
    }
}

RouteMode SecureAppSettingsRepository::routeMode() const
{
    return static_cast<RouteMode>(value("Conf/routeMode", 0).toInt());
}

void SecureAppSettingsRepository::setRouteMode(RouteMode mode)
{
    setValue("Conf/routeMode", static_cast<int>(mode));
    emit routeModeChanged(mode);
}

QVariantMap SecureAppSettingsRepository::vpnSites(RouteMode mode) const
{
    return value("Conf/" + routeModeString(mode)).toMap();
}

QStringList SecureAppSettingsRepository::siteIpList(const QVariant &value)
{
    // QVariant::toStringList() handles both a QStringList/QVariantList and a single QString
    // (a single string is returned as a one-element list), which covers the legacy format.
    QStringList result = value.toStringList();
    result.removeAll(QString());
    result.removeDuplicates();
    return result;
}

void SecureAppSettingsRepository::setVpnSites(RouteMode mode, const QVariantMap &sites)
{
    setValue("Conf/" + routeModeString(mode), sites);
}

bool SecureAppSettingsRepository::addVpnSite(RouteMode mode, const QString &site, const QStringList &ips)
{
    QVariantMap sites = vpnSites(mode);
    const bool siteExisted = sites.contains(site);

    if (siteExisted && ips.isEmpty())
        return false;

    QStringList mergedIps = siteIpList(sites.value(site));
    bool changed = !siteExisted;
    for (const QString &ip : ips) {
        if (!ip.isEmpty() && !mergedIps.contains(ip)) {
            mergedIps.append(ip);
            changed = true;
        }
    }

    if (!changed)
        return false;

    sites.insert(site, mergedIps);
    setVpnSites(mode, sites);
    emit sitesChanged(mode);
    return true;
}

void SecureAppSettingsRepository::addVpnSites(RouteMode mode, const QMap<QString, QStringList> &sites)
{
    QVariantMap allSites = vpnSites(mode);
    for (auto i = sites.constBegin(); i != sites.constEnd(); ++i) {
        const QString &site = i.key();

        QStringList mergedIps = siteIpList(allSites.value(site));
        for (const QString &ip : i.value()) {
            if (!ip.isEmpty() && !mergedIps.contains(ip))
                mergedIps.append(ip);
        }

        allSites.insert(site, mergedIps);
    }

    setVpnSites(mode, allSites);
    emit sitesChanged(mode);
}

void SecureAppSettingsRepository::removeVpnSite(RouteMode mode, const QString &site)
{
    QVariantMap sites = vpnSites(mode);
    if (!sites.contains(site))
        return;

    sites.remove(site);
    setVpnSites(mode, sites);
    emit sitesChanged(mode);
}

void SecureAppSettingsRepository::removeAllVpnSites(RouteMode mode)
{
    setVpnSites(mode, QVariantMap());
    emit sitesChanged(mode);
}

bool SecureAppSettingsRepository::isSitesSplitTunnelingEnabled() const
{
    return value("Conf/sitesSplitTunnelingEnabled", false).toBool();
}

void SecureAppSettingsRepository::setSitesSplitTunnelingEnabled(bool enabled)
{
    setValue("Conf/sitesSplitTunnelingEnabled", enabled);
    emit sitesSplitTunnelingEnabledChanged(enabled);
}

namespace {
    QString appsRouteModeString(AppsRouteMode mode) {
        switch (mode) {
        case AppsRouteMode::VpnAllApps: return "AllApps";
        case AppsRouteMode::VpnOnlyForwardApps: return "ForwardApps";
        case AppsRouteMode::VpnAllExceptApps: return "ExceptApps";
        }
        return QString();
    }
}

AppsRouteMode SecureAppSettingsRepository::appsRouteMode() const
{
    return static_cast<AppsRouteMode>(value("Conf/appsRouteMode", 0).toInt());
}

void SecureAppSettingsRepository::setAppsRouteMode(AppsRouteMode mode)
{
    setValue("Conf/appsRouteMode", static_cast<int>(mode));
    emit appsRouteModeChanged(mode);
}

QVector<InstalledAppInfo> SecureAppSettingsRepository::vpnApps(AppsRouteMode mode) const
{
    QVector<InstalledAppInfo> apps;
    auto appsArray = value("Conf/" + appsRouteModeString(mode)).toJsonArray();
    for (const auto &app : appsArray) {
        InstalledAppInfo appInfo;
        appInfo.appName = app.toObject().value("appName").toString();
        appInfo.packageName = app.toObject().value("packageName").toString();
        appInfo.appPath = app.toObject().value("appPath").toString();

        apps.push_back(appInfo);
    }
    return apps;
}

void SecureAppSettingsRepository::setVpnApps(AppsRouteMode mode, const QVector<InstalledAppInfo> &apps)
{
    QJsonArray appsArray;
    for (const auto &app : apps) {
        QJsonObject appInfo;
        appInfo.insert("appName", app.appName);
        appInfo.insert("packageName", app.packageName);
        appInfo.insert("appPath", app.appPath);
        appsArray.push_back(appInfo);
    }
    setValue("Conf/" + appsRouteModeString(mode), appsArray);
    emit appsChanged(mode);
}

bool SecureAppSettingsRepository::isAppsSplitTunnelingEnabled() const
{
    return value("Conf/appsSplitTunnelingEnabled", false).toBool();
}

void SecureAppSettingsRepository::setAppsSplitTunnelingEnabled(bool enabled)
{
    setValue("Conf/appsSplitTunnelingEnabled", enabled);
    emit appsSplitTunnelingEnabledChanged(enabled);
}

bool SecureAppSettingsRepository::isKillSwitchEnabled() const
{
    return value("Conf/killSwitchEnabled", true).toBool();
}

void SecureAppSettingsRepository::setKillSwitchEnabled(bool enabled)
{
    setValue("Conf/killSwitchEnabled", enabled);
}

bool SecureAppSettingsRepository::isStrictKillSwitchEnabled() const
{
    return value("Conf/strictKillSwitchEnabled", false).toBool();
}

void SecureAppSettingsRepository::setStrictKillSwitchEnabled(bool enabled)
{
    setValue("Conf/strictKillSwitchEnabled", enabled);
}

bool SecureAppSettingsRepository::isSaveLogs() const
{
    return value("Conf/saveLogs", false).toBool();
}

void SecureAppSettingsRepository::setSaveLogs(bool enabled)
{
    setValue("Conf/saveLogs", enabled);
    emit saveLogsChanged(enabled);
}

QDateTime SecureAppSettingsRepository::getLogEnableDate() const
{
    return value("Conf/logEnableDate").toDateTime();
}

void SecureAppSettingsRepository::setLogEnableDate(const QDateTime &date)
{
    setValue("Conf/logEnableDate", date);
}

QString SecureAppSettingsRepository::getInstallationUuid(bool createIfNotExists) const
{
    auto uuid = value("Conf/installationUuid", "").toString();
    if (createIfNotExists && uuid.isEmpty()) {
        uuid = QUuid::createUuid().toString();
        uuid.remove(0, 1);
        uuid.chop(1);
        const_cast<SecureAppSettingsRepository*>(this)->setValue("Conf/installationUuid", uuid);
    } else if (uuid.contains("{") && uuid.contains("}")) {
        uuid.remove(0, 1);
        uuid.chop(1);
        const_cast<SecureAppSettingsRepository*>(this)->setValue("Conf/installationUuid", uuid);
    }
    return uuid;
}

void SecureAppSettingsRepository::clearSettings()
{
    auto uuid = getInstallationUuid(false);
    m_settings->clearSettings();
    m_settings->setValue("Conf/installationUuid", uuid);
    emit settingsCleared();
}
