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
        Q_EMIT permissionUpdateDone(replyObject);
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("VideoConferenceJoinJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
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
    //    if (mPermissions.isEmpty()) {
    //        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "VideoConferenceJoinJob: mPermissions is empty";
    //        return false;
    //    }
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
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
