/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelopenjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

using namespace RocketChatRestApi;
ChannelOpenJob::ChannelOpenJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelOpenJob::~ChannelOpenJob() = default;

bool ChannelOpenJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelOpenJob::start");
    submitPostRequest(json());

    return true;
}

void ChannelOpenJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ChannelOpenJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT channelOpenDone(replyObject, channelGroupInfo());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ChannelOpenJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ChannelOpenJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelOpenJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelOpenJob: RoomId and RoomName are empty";
        return false;
    }
    return true;
}

QJsonDocument ChannelOpenJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChannelOpenJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    if (str == QLatin1String("error-room-not-found")) {
        return i18n("The required channel is not found.");
    }
    return ChannelGroupBaseJob::errorMessage(str, detail);
}

QNetworkRequest ChannelOpenJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsOpen);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_channelopenjob.cpp"
