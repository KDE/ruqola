/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencelistjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
VideoConferenceListJob::VideoConferenceListJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

VideoConferenceListJob::~VideoConferenceListJob() = default;

bool VideoConferenceListJob::requireHttpAuthentication() const
{
    return true;
}

bool VideoConferenceListJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "VideoConferenceListJob: mRoomId is empty";
        return false;
    }
    return true;
}

bool VideoConferenceListJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start VideoConferenceListJob";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("VideoConferenceListJob: Ask for capabilities"_ba);
    return true;
}

void VideoConferenceListJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("VideoConferenceListJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT videoConferenceInfoDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("VideoConferenceListJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString VideoConferenceListJob::roomId() const
{
    return mRoomId;
}

void VideoConferenceListJob::setRoomId(const QString &newRoomId)
{
    mRoomId = newRoomId;
}

QNetworkRequest VideoConferenceListJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::VideoConferenceList);

    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"roomId"_s, mRoomId);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_videoconferencelistjob.cpp"
