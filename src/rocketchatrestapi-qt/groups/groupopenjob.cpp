/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupopenjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

using namespace RocketChatRestApi;
GroupOpenJob::GroupOpenJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

GroupOpenJob::~GroupOpenJob() = default;

bool GroupOpenJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("GroupOpenJob::start");
    submitPostRequest(json());

    return true;
}

void GroupOpenJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GroupOpenJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT groupOpenDone(replyObject, channelGroupInfo());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("GroupOpenJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool GroupOpenJob::requireHttpAuthentication() const
{
    return true;
}

bool GroupOpenJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupOpenJob: RoomId and RoomName are empty";
        return false;
    }
    return true;
}

QJsonDocument GroupOpenJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest GroupOpenJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsOpen);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_groupopenjob.cpp"
