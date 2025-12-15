/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencejoinjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
VideoConferenceJoinJob::VideoConferenceJoinJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

VideoConferenceJoinJob::~VideoConferenceJoinJob() = default;

bool VideoConferenceJoinJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("VideoConferenceJoinJob::start");
    submitPostRequest(json());

    return true;
}

void VideoConferenceJoinJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("VideoConferenceJoinJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT videoConferenceJoinDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("VideoConferenceJoinJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

VideoConferenceJoinJob::VideoConferenceJoinInfo VideoConferenceJoinJob::info() const
{
    return mInfo;
}

void VideoConferenceJoinJob::setInfo(const VideoConferenceJoinInfo &newInfo)
{
    mInfo = newInfo;
}

bool VideoConferenceJoinJob::requireHttpAuthentication() const
{
    return true;
}

bool VideoConferenceJoinJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "VideoConferenceJoinJob: mInfo is empty";
        return false;
    }
    return true;
}

QNetworkRequest VideoConferenceJoinJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::VideoConferenceJoin);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument VideoConferenceJoinJob::json() const
{
    QJsonObject jsonObj;
    // {"callId":"639496df4ef3f3baa9658f0c","state":{"mic":false,"cam":false}}
    jsonObj["callId"_L1] = mInfo.callId;
    QJsonObject jsonStateObj;
    jsonStateObj["mic"_L1] = mInfo.useMicro;
    jsonStateObj["cam"_L1] = mInfo.useCamera;
    jsonObj["state"_L1] = jsonStateObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool VideoConferenceJoinJob::VideoConferenceJoinInfo::isValid() const
{
    return !callId.isEmpty();
}

#include "moc_videoconferencejoinjob.cpp"
