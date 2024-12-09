/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferenceprovidersjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
VideoConferenceProvidersJob::VideoConferenceProvidersJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

VideoConferenceProvidersJob::~VideoConferenceProvidersJob() = default;

bool VideoConferenceProvidersJob::requireHttpAuthentication() const
{
    return true;
}

bool VideoConferenceProvidersJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start VideoConferenceProvidersJob";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("VideoConferenceProvidersJob: Ask for provider lists"));
    return true;
}

void VideoConferenceProvidersJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("VideoConferenceProvidersJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        // {"data":[{"key":"jitsi","label":"Jitsi"}]
        Q_EMIT videoConferenceProvidersDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("VideoConferenceProvidersJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QNetworkRequest VideoConferenceProvidersJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::VideoConferenceProviders);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_videoconferenceprovidersjob.cpp"
