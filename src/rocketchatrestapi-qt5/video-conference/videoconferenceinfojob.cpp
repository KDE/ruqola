/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferenceinfojob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>

using namespace RocketChatRestApi;
VideoConferenceInfoJob::VideoConferenceInfoJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

VideoConferenceInfoJob::~VideoConferenceInfoJob() = default;

bool VideoConferenceInfoJob::requireHttpAuthentication() const
{
    return true;
}

bool VideoConferenceInfoJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mCallId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "VideoConferenceInfoJob: mCallId is empty";
        return false;
    }
    return true;
}

bool VideoConferenceInfoJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start VideoConferenceInfoJob";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("VideoConferenceInfoJob: Ask for capabilities"));
    return true;
}

void VideoConferenceInfoJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("VideoConferenceInfoJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT videoConferenceInfoDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("VideoConferenceInfoJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString VideoConferenceInfoJob::callId() const
{
    return mCallId;
}

void VideoConferenceInfoJob::setCallId(const QString &newCallId)
{
    mCallId = newCallId;
}

QNetworkRequest VideoConferenceInfoJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::VideoConferenceInfo);

    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("callId"), mCallId);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

#include "moc_videoconferenceinfojob.cpp"
