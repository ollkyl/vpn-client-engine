#include <QCommandLineParser>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>

#include <windows.h>

#include "core/utils/errorStrings.h"
#include "version.h"
#include "vpnEngine.h"

namespace
{
    QTextStream &out()
    {
        static QTextStream stream(stdout);
        return stream;
    }

    QTextStream &err()
    {
        static QTextStream stream(stderr);
        return stream;
    }

    void requestQuit()
    {
        QMetaObject::invokeMethod(QCoreApplication::instance(), &QCoreApplication::quit, Qt::QueuedConnection);
    }

    BOOL WINAPI consoleCtrlHandler(DWORD ctrlType)
    {
        switch (ctrlType) {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
        case CTRL_CLOSE_EVENT:
            requestQuit();
            return TRUE;
        default:
            return FALSE;
        }
    }

    void installQuitHandler()
    {
        SetConsoleCtrlHandler(consoleCtrlHandler, TRUE);
    }

    int fail(ErrorCode errorCode)
    {
        err() << errorString(errorCode) << Qt::endl;
        return 1;
    }

    int importConfig(VpnEngine &engine, const QString &source)
    {
        QString data = source;
        QFile file(source);
        if (file.exists()) {
            if (!file.open(QIODevice::ReadOnly)) {
                err() << "Cannot read " << source << Qt::endl;
                return 1;
            }
            data = QString::fromUtf8(file.readAll());
        }

        const auto parsed = engine.importer()->extractConfigFromData(data.trimmed());
        if (parsed.errorCode != ErrorCode::NoError) {
            return fail(parsed.errorCode);
        }
        if (!parsed.maliciousWarningText.isEmpty()) {
            err() << "Warning: OpenVPN configs can run scripts, import it only if you trust its source" << Qt::endl;
        }

        const QString serverId = engine.importer()->importConfig(parsed.config);
        if (serverId.isEmpty()) {
            return fail(ErrorCode::ImportInvalidConfigError);
        }
        out() << "Imported server " << serverId << Qt::endl;
        return 0;
    }

    int listServers(const VpnEngine &engine)
    {
        const QString defaultId = engine.servers()->getDefaultServerId();
        const auto servers = engine.servers()->serverDescriptions();
        if (servers.isEmpty()) {
            out() << "No servers" << Qt::endl;
            return 0;
        }
        for (const auto &server : servers) {
            out() << (server.serverId == defaultId ? "* " : "  ") << server.serverId << "  " << server.name << "  "
                  << server.hostName << "  " << server.protocolName << Qt::endl;
        }
        return 0;
    }

    int removeServer(VpnEngine &engine, const QString &serverId)
    {
        if (engine.servers()->indexOfServerId(serverId) < 0) {
            err() << "Unknown server " << serverId << Qt::endl;
            return 1;
        }
        engine.servers()->removeServer(serverId);
        return 0;
    }

    // Connects and keeps the tunnel up until Ctrl+C
    int connectAndWait(QCoreApplication &app, VpnEngine &engine, const QString &serverId)
    {
        int exitCode = 0;
        bool disconnecting = false;

        QObject::connect(&engine, &VpnEngine::connectionStateChanged, &app, [&](Vpn::ConnectionState state) {
            out() << VpnProtocol::textConnectionState(state) << Qt::endl;
            if (disconnecting && (state == Vpn::ConnectionState::Disconnected || state == Vpn::ConnectionState::Error)) {
                app.exit(exitCode);
            }
        });
        QObject::connect(&engine, &VpnEngine::connectionErrorOccurred, &app, [&](ErrorCode errorCode) {
            exitCode = fail(errorCode);
            disconnecting = true;
            engine.disconnectFromServer();
        });

        const ErrorCode errorCode = engine.connectToServer(serverId);
        if (errorCode != ErrorCode::NoError) {
            return fail(errorCode);
        }

        installQuitHandler();
        app.exec();

        // Ctrl+C: tear the tunnel down before exiting
        if (!disconnecting && engine.connectionState() != Vpn::ConnectionState::Disconnected) {
            disconnecting = true;
            engine.disconnectFromServer();
            app.exec();
        }
        return exitCode;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setApplicationVersion(APP_VERSION);

    QCommandLineParser parser;
    parser.setApplicationDescription("VPN engine console client");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("command",
                                 "import <file|key>   import a config file or a vpn://, vless://, ... key\n"
                                 "list                list servers, * marks the default one\n"
                                 "remove <id>         remove a server\n"
                                 "connect [id]        connect to a server (default if omitted), Ctrl+C disconnects");
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    if (args.isEmpty()) {
        parser.showHelp(1);
    }

    VpnEngine engine;

    const QString command = args.at(0);
    const QString argument = args.value(1);

    if (command == "import" && !argument.isEmpty()) {
        return importConfig(engine, argument);
    }
    if (command == "list") {
        return listServers(engine);
    }
    if (command == "remove" && !argument.isEmpty()) {
        return removeServer(engine, argument);
    }
    if (command == "connect") {
        return connectAndWait(app, engine, argument);
    }

    parser.showHelp(1);
}
