/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencestartjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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

void VideoConferenceStartJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("VideoConferenceStartJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT videoConferenceStartDone(replyObject["data"_L1].toObject());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("VideoConferenceStartJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    jsonObj["roomId"_L1] = QLatin1StringView(mInfo.roomId);
    jsonObj["allowRinging"_L1] = mInfo.allowRinging;
    if (!mInfo.title.isEmpty()) {
        jsonObj["title"_L1] = mInfo.title;
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool VideoConferenceStartJob::VideoConferenceStartInfo::isValid() const
{
    return !roomId.isEmpty();
}

#include "moc_videoconferencestartjob.cpp"
