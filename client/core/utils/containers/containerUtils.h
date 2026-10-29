#ifndef CONTAINERUTILS_H
#define CONTAINERUTILS_H

#include <QList>
#include <QMap>
#include <QString>
#include <QJsonObject>

#include "core/utils/containerEnum.h"
#include "core/utils/protocolEnum.h"
#include "core/protocols/protocolUtils.h"
#include "core/utils/constants/configKeys.h"

namespace amnezia
{
    namespace ContainerUtils
    {
        DockerContainer containerFromString(const QString &container);
        QString containerToString(DockerContainer container);
        QString containerTypeToString(DockerContainer c);
        QString containerTypeToProtocolString(DockerContainer c);

        QList<DockerContainer> allContainers();

        QMap<DockerContainer, QString> containerHumanNames();

        ServiceType containerService(DockerContainer c);

        // binding between a container and its main protocol
        Proto defaultProtocol(DockerContainer c);

        bool isSupportedByCurrentPlatform(DockerContainer c);

        bool isAwgContainer(DockerContainer container);

        bool isUnsupportedContainer(DockerContainer container);

        QJsonObject getProtocolConfigFromContainer(const Proto protocol, const QJsonObject &containerConfig);
    }
}

#endif // CONTAINERUTILS_H
