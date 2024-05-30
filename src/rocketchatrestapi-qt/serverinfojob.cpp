/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverinfojob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
ServerInfoJob::ServerInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ServerInfoJob::~ServerInfoJob() = default;

bool ServerInfoJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start server info job";
        deleteLater();
        return false;
    }

    submitGetRequest();
    addStartRestApiInfo("ServerInfoJob::start");
    return true;
}

QNetworkRequest ServerInfoJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ServerInfo, QString(), RestApiUtil::RestApiUrlExtensionType::NoExtension);
    QNetworkRequest request(url);
    addRequestAttribute(request, false);

    return request;
}

bool ServerInfoJob::requireHttpAuthentication() const
{
    return false;
}

void ServerInfoJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    // TODO send replyObject too. Need by administrator server info.
    if (replyObject["success"_L1].toBool()) {
        const QString versionStr = replyObject.value("version"_L1).toString();
        addLoggerInfo(QByteArrayLiteral("ServerInfoJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT serverInfoDone(versionStr, replyObject);
    } else {
        Q_EMIT serverInfoFailed(false);
        addLoggerWarning(QByteArrayLiteral("ServerInfoJob::slotServerInfoFinished: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        emitFailedMessage(replyErrorString, replyObject);
    }
}

#include "moc_serverinfojob.cpp"
