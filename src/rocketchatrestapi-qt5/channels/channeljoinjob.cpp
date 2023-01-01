/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channeljoinjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

using namespace RocketChatRestApi;
ChannelJoinJob::ChannelJoinJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelJoinJob::~ChannelJoinJob() = default;

bool ChannelJoinJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelJoinJob::start");
    submitPostRequest(json());

    return true;
}

void ChannelJoinJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ChannelJoinJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT setChannelJoinDone(channelGroupInfo());
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("ChannelJoinJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        // Invalid password
        const QString errorType = replyObject[QStringLiteral("errorType")].toString();
        if (errorType == QLatin1String("error-code-invalid")) {
            Q_EMIT missingChannelPassword(channelGroupInfo());
        } else if (errorType == QLatin1String("error-room-archived")) {
            Q_EMIT openArchivedRoom(channelGroupInfo());
        }
    }
}

QString ChannelJoinJob::joinCode() const
{
    return mJoinCode;
}

void ChannelJoinJob::setJoinCode(const QString &joinCode)
{
    mJoinCode = joinCode;
}

bool ChannelJoinJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelJoinJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelJoinJob: RoomId and RoomName are empty";
        return false;
    }
    return true;
}

QJsonDocument ChannelJoinJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    if (!mJoinCode.isEmpty()) {
        jsonObj[QLatin1String("joinCode")] = mJoinCode;
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChannelJoinJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsJoin);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QString ChannelJoinJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    if (str == QLatin1String("error-room-not-found")) {
        return i18n("The required \'%1\' param provided does not match any channel", channelGroupInfo().identifier);
    } else if (str == QLatin1String("error-code-invalid")) {
        return i18n("The room required a password.");
    }
    return ChannelGroupBaseJob::errorMessage(str, detail);
}
