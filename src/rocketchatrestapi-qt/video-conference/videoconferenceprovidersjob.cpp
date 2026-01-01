/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferenceprovidersjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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

    addStartRestApiInfo("VideoConferenceProvidersJob: Ask for provider lists"_ba);
    return true;
}

void VideoConferenceProvidersJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("VideoConferenceProvidersJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        // {"data":[{"key":"jitsi","label":"Jitsi"}]
        Q_EMIT videoConferenceProvidersDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("VideoConferenceProvidersJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
