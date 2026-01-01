/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channeljoinjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
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

void ChannelJoinJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ChannelJoinJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT setChannelJoinDone(channelGroupInfo());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ChannelJoinJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
        // Invalid password
        const QString errorType = replyObject["errorType"_L1].toString();
        if (errorType == "error-code-invalid"_L1) {
            Q_EMIT missingChannelPassword(channelGroupInfo());
        } else if (errorType == "error-room-archived"_L1) {
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
        jsonObj["joinCode"_L1] = mJoinCode;
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
    if (str == "error-room-not-found"_L1) {
        return i18n("The required \'%1\' param provided does not match any channel", channelGroupInfo().identifier);
    } else if (str == "error-code-invalid"_L1) {
        return i18n("The room required a password.");
    }
    return ChannelGroupBaseJob::errorMessage(str, detail);
}

#include "moc_channeljoinjob.cpp"
