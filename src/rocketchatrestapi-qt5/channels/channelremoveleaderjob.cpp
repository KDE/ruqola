/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelremoveleaderjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelRemoveLeaderJob::ChannelRemoveLeaderJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelRemoveLeaderJob::~ChannelRemoveLeaderJob() = default;

bool ChannelRemoveLeaderJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelRemoveLeaderJob::start");
    submitPostRequest(json());

    return true;
}

void ChannelRemoveLeaderJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ChannelRemoveLeaderJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT removeLeaderDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ChannelRemoveLeaderJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ChannelRemoveLeaderJob::removeUserId() const
{
    return mRemoveUserId;
}

void ChannelRemoveLeaderJob::setRemoveUserId(const QString &removeUserId)
{
    mRemoveUserId = removeUserId;
}

bool ChannelRemoveLeaderJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelRemoveLeaderJob::canStart() const
{
    if (mRemoveUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelRemoveLeaderJob: remove userid is empty";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelRemoveLeaderJob: RoomId and RoomName are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChannelRemoveLeaderJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj[QLatin1String("userId")] = removeUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChannelRemoveLeaderJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsRemoveLeader);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
