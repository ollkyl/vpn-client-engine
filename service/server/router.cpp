#include "router.h"

#include "router_win.h"

int Router::routeAddList(const QString &gw, const QStringList &ips)
{
    return RouterWin::Instance().routeAddList(gw, ips);
}

bool Router::clearSavedRoutes()
{
    return RouterWin::Instance().clearSavedRoutes();
}

int Router::routeDeleteList(const QString &gw, const QStringList &ips)
{
    return RouterWin::Instance().routeDeleteList(gw, ips);
}

bool Router::flushDns()
{
    return RouterWin::Instance().flushDns();
}

void Router::resetIpStack()
{
    RouterWin::Instance().resetIpStack();
}

bool Router::createTun(const QString &dev, const QString &subnet)
{
    return RouterWin::Instance().createTun(dev, subnet);
    return true;
};

bool Router::deleteTun(const QString &dev)
{
    return true;
};

bool Router::updateResolvers(const QString& ifname, const QList<QHostAddress>& resolvers)
{
    return RouterWin::Instance().updateResolvers(ifname, resolvers);
}

bool Router::restoreResolvers() {
    return RouterWin::Instance().restoreResolvers();
}

bool Router::StopRoutingIpv6()
{
    return RouterWin::Instance().StopRoutingIpv6();
}

bool Router::StartRoutingIpv6()
{
    return RouterWin::Instance().StartRoutingIpv6();
}

