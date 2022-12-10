/*
   SPDX-FileCopyrightText:2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencejoinjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
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

void VideoConferenceJoinJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("VideoConferenceJoinJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT videoConferenceJoinDone(replyObject);
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("VideoConferenceJoinJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
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
    jsonObj[QLatin1String("callId")] = mInfo.callId;
    QJsonObject jsonStateObj;
    jsonStateObj[QLatin1String("mic")] = mInfo.useMicro;
    jsonStateObj[QLatin1String("cam")] = mInfo.useCamera;
    jsonObj[QLatin1String("state")] = jsonStateObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool VideoConferenceJoinJob::VideoConferenceJoinInfo::isValid() const
{
    return !callId.isEmpty();
}
