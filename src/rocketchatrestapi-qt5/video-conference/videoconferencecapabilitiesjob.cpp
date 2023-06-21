/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencecapabilitiesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
VideoConferenceCapabilitiesJob::VideoConferenceCapabilitiesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

VideoConferenceCapabilitiesJob::~VideoConferenceCapabilitiesJob() = default;

bool VideoConferenceCapabilitiesJob::requireHttpAuthentication() const
{
    return true;
}

bool VideoConferenceCapabilitiesJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start VideoConferenceCapabilitiesJob";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("VideoConferenceCapabilitiesJob: Ask for capabilities"));
    return true;
}

void VideoConferenceCapabilitiesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("VideoConferenceCapabilitiesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT videoConferenceCapabilitiesDone(replyObject);
    } else {
        const QString errorType = replyObject[QStringLiteral("error")].toString();
        if (errorType == QLatin1String("no-videoconf-provider-app")) {
            Q_EMIT noVideoConferenceProviderApps();
            addLoggerWarning(QByteArrayLiteral("VideoConferenceCapabilitiesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        } else {
            emitFailedMessage(replyErrorString, replyObject);
            addLoggerWarning(QByteArrayLiteral("VideoConferenceCapabilitiesJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
    }
}

QNetworkRequest VideoConferenceCapabilitiesJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::VideoConferenceCapabilities);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

#include "moc_videoconferencecapabilitiesjob.cpp"
