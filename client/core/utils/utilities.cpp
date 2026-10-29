#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>

#include "core/utils/utilities.h"

QString printErrorMessage(DWORD errorCode) {
    LPVOID lpMsgBuf;

    DWORD dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS;

    DWORD dwLanguageId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);

    FormatMessageW(
        dwFlags,
        NULL,
        errorCode,
        dwLanguageId,
        (LPWSTR)&lpMsgBuf,
        0,
        NULL
        );

    QString errorMsg = QString::fromWCharArray((LPCWSTR)lpMsgBuf);
    LocalFree(lpMsgBuf);
    return errorMsg.trimmed();
}

QString Utils::VerifyJsonString(const QString &source)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(source.toUtf8(), &error);
    Q_UNUSED(doc)

    if (error.error == QJsonParseError::NoError) {
        return "";
    } else {
        qDebug() << "WARNING: Json parse returns: " + error.errorString();
        return error.errorString();
    }
}

QJsonObject Utils::JsonFromString(const QString &string)
{
    auto removeComment = string.trimmed();
    if (removeComment != string.trimmed()) {
        qDebug() << "Some comments have been removed from the json.";
    }
    QJsonDocument doc = QJsonDocument::fromJson(removeComment.toUtf8());
    return doc.object();
}

QString Utils::SafeBase64Decode(QString string)
{
    QByteArray ba = string.replace(QChar('-'), QChar('+')).replace(QChar('_'), QChar('/')).toUtf8();
    return QByteArray::fromBase64(ba, QByteArray::Base64Option::OmitTrailingEquals);
}

QString Utils::JsonToString(const QJsonObject &json, QJsonDocument::JsonFormat format)
{
    QJsonDocument doc;
    doc.setObject(json);
    return doc.toJson(format);
}

QString Utils::JsonToString(const QJsonArray &array, QJsonDocument::JsonFormat format)
{
    QJsonDocument doc;
    doc.setArray(array);
    return doc.toJson(format);
}

bool Utils::initializePath(const QString &path)
{
    QDir dir;
    if (!dir.mkpath(path)) {
        qWarning().noquote() << QString("Cannot initialize path: '%1'").arg(path);
        return false;
    }
    return true;
}

QString Utils::executable(const QString &baseName, bool absPath)
{
    QString ext;
    ext = ".exe";
    const QString fileName = baseName + ext;
    if (!absPath) {
        return fileName;
    }
    return QCoreApplication::applicationDirPath() + "/" + fileName;
}

bool Utils::processIsRunning(const QString &fileName, const bool fullFlag)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        qWarning() << "Utils::processIsRunning error CreateToolhelp32Snapshot";
        return false;
    }

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    if (!Process32FirstW(hSnapshot, &pe32)) {
        CloseHandle(hSnapshot);
        qWarning() << "Utils::processIsRunning error Process32FirstW";
        return false;
    }

    do {
        QString exeFile = QString::fromWCharArray(pe32.szExeFile);

        if (exeFile.compare(fileName, Qt::CaseInsensitive) == 0) {
            CloseHandle(hSnapshot);
            return true;
        }
    } while (Process32NextW(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
    return false;

}

bool Utils::killProcessByName(const QString &name)
{
    qDebug().noquote() << "Kill process" << name;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return false;

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    bool success = false;

    if (Process32FirstW(hSnapshot, &pe32)) {
        do {
            QString exeFile = QString::fromWCharArray(pe32.szExeFile);

            if (exeFile.compare(name, Qt::CaseInsensitive) == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                if (hProcess != NULL) {
                    if (TerminateProcess(hProcess, 0)) {
                        success = true;
                    } else {
                        DWORD error = GetLastError();
                        qCritical() << "Can't terminate process" << exeFile << "(PID:" << pe32.th32ProcessID << "). Error:" << printErrorMessage(error);
                    }
                    CloseHandle(hProcess);
                } else {
                    DWORD error = GetLastError();
                    qCritical() << "Can't open process for termination" << exeFile << "(PID:" << pe32.th32ProcessID << "). Error:" << printErrorMessage(error);
                }
            }
        } while (Process32NextW(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return success;
}

QString Utils::openVpnExecPath()
{
    return Utils::executable("openvpn", true);
}

QString Utils::wireguardExecPath()
{
    return Utils::executable("wireguard/wireguard-service", true);
}

QString Utils::certUtilPath()
{
    QString winPath = QString::fromUtf8(qgetenv("windir"));
    return winPath + "\\system32\\certutil.exe";
}

QString Utils::tun2socksPath()
{
    return Utils::executable("tun2socks", true);
}
