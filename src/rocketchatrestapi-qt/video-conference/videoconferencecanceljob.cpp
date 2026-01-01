/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencecanceljob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
VideoConferenceCancelJob::VideoConferenceCancelJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

VideoConferenceCancelJob::~VideoConferenceCancelJob() = default;

bool VideoConferenceCancelJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("VideoConferenceCancelJob::start");
    submitPostRequest(json());

    return true;
}

void VideoConferenceCancelJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("VideoConferenceCancelJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT videoConferenceCancelDone(replyObject["data"_L1].toObject());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("VideoConferenceCancelJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString VideoConferenceCancelJob::callId() const
{
    return mCallId;
}

void VideoConferenceCancelJob::setCallId(const QString &newCallId)
{
    mCallId = newCallId;
}

bool VideoConferenceCancelJob::requireHttpAuthentication() const
{
    return true;
}

bool VideoConferenceCancelJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mCallId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "VideoConferenceCancelJob: mCallId is invalid";
        return false;
    }
    return true;
}

QNetworkRequest VideoConferenceCancelJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::VideoConferenceCancel);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument VideoConferenceCancelJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["callId"_L1] = mCallId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_videoconferencecanceljob.cpp"
