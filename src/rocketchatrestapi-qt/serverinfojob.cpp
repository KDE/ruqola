/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverinfojob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
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
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ServerInfo, QString(), false);
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
    if (replyObject[QLatin1String("success")].toBool()) {
        const QString versionStr = replyObject.value(QStringLiteral("version")).toString();
        addLoggerInfo(QByteArrayLiteral("ServerInfoJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT serverInfoDone(versionStr, replyObject);
    } else {
        Q_EMIT serverInfoFailed(false);
        addLoggerWarning(QByteArrayLiteral("ServerInfoJob::slotServerInfoFinished: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        emitFailedMessage(replyErrorString, replyObject);
    }
}

#include "moc_serverinfojob.cpp"
