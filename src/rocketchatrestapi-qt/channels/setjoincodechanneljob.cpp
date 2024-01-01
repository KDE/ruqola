/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setjoincodechanneljob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
SetJoinCodeChannelJob::SetJoinCodeChannelJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

SetJoinCodeChannelJob::~SetJoinCodeChannelJob() = default;

bool SetJoinCodeChannelJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SetJoinCodeChannelJob::start");
    submitPostRequest(json());

    return true;
}

void SetJoinCodeChannelJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("SetJoinCodeChannelJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT setJoinCodeDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("SetJoinCodeChannelJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString SetJoinCodeChannelJob::joinCode() const
{
    return mJoinCode;
}

void SetJoinCodeChannelJob::setJoinCode(const QString &joinCode)
{
    mJoinCode = joinCode;
}

bool SetJoinCodeChannelJob::requireHttpAuthentication() const
{
    return true;
}

bool SetJoinCodeChannelJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetJoinCodeChannelJob: RoomId and RoomName are empty";
        return false;
    }
    if (mJoinCode.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetJoinCodeChannelJob: JoinCode is empty";
        return false;
    }
    return true;
}

QJsonDocument SetJoinCodeChannelJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj[QLatin1String("joinCode")] = joinCode();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest SetJoinCodeChannelJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetJoinCode);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_setjoincodechanneljob.cpp"
