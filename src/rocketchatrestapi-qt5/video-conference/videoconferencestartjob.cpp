/*
   SPDX-FileCopyrightText:2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencestartjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
VideoConferenceStartJob::VideoConferenceStartJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

VideoConferenceStartJob::~VideoConferenceStartJob() = default;

bool VideoConferenceStartJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("VideoConferenceStartJob::start");
    submitPostRequest(json());

    return true;
}

void VideoConferenceStartJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("VideoConferenceStartJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT videoConferenceStartDone(replyObject);
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("VideoConferenceStartJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

VideoConferenceStartJob::VideoConferenceStartInfo VideoConferenceStartJob::info() const
{
    return mInfo;
}

void VideoConferenceStartJob::setInfo(const VideoConferenceStartInfo &newInfo)
{
    mInfo = newInfo;
}

bool VideoConferenceStartJob::requireHttpAuthentication() const
{
    return true;
}

bool VideoConferenceStartJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "VideoConferenceStartJob: mInfo is invalid";
        return false;
    }
    return true;
}

QNetworkRequest VideoConferenceStartJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::VideoConferenceStart);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument VideoConferenceStartJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = mInfo.roomId;
    jsonObj[QLatin1String("allowRinging")] = mInfo.allowRinging;
    if (!mInfo.title.isEmpty()) {
        jsonObj[QLatin1String("title")] = mInfo.title;
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool VideoConferenceStartJob::VideoConferenceStartInfo::isValid() const
{
    return !roomId.isEmpty();
}
