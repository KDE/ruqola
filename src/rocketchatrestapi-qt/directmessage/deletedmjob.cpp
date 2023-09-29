/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deletedmjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
DeleteDmJob::DeleteDmJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

DeleteDmJob::~DeleteDmJob() = default;

bool DeleteDmJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("DeleteDmJob::start");
    submitPostRequest(json());

    return true;
}

void DeleteDmJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("DeleteDmJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        qDebug() << " replyObject " << replyObject;
        Q_EMIT deleteDirectMessagesDone(channelGroupInfo().identifier);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("DeleteDmJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool DeleteDmJob::requireHttpAuthentication() const
{
    return true;
}

bool DeleteDmJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "DeleteDmJob: RoomId and RoomName are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument DeleteDmJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest DeleteDmJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ImDelete);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_deletedmjob.cpp"
