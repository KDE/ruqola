/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinvitejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
ChannelInviteJob::ChannelInviteJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelInviteJob::~ChannelInviteJob() = default;

bool ChannelInviteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelInviteJob::start");
    submitPostRequest(json());

    return true;
}

void ChannelInviteJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ChannelInviteJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT inviteDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ChannelInviteJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

ChannelInviteJob::ChannelInviteInfo ChannelInviteJob::channelInviteInfo() const
{
    return mChannelInviteInfo;
}

void ChannelInviteJob::setChannelInviteInfo(const ChannelInviteInfo &newChannelInviteInfo)
{
    mChannelInviteInfo = newChannelInviteInfo;
}

bool ChannelInviteJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelInviteJob::canStart() const
{
    if (!mChannelInviteInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelInviteJob: mChannelInviteInfo is invalid";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelInviteJob: RoomId and RoomName are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChannelInviteJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    switch (mChannelInviteInfo.channelGroupInfoType) {
    case ChannelInviteJob::ChannelInviteInfoType::Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelInviteJob: ChannelInviteJob::ChannelInviteInfoType::Unknown it's a bug";
        break;
    case ChannelInviteJob::ChannelInviteInfoType::UserId:
        jsonObj["userId"_L1] = mChannelInviteInfo.identifier;
        break;
    case ChannelInviteJob::ChannelInviteInfoType::UserName:
        jsonObj["userName"_L1] = mChannelInviteInfo.identifier;
        break;
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChannelInviteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsInvite);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QString ChannelInviteJob::errorMessage(const QString &str, const QJsonObject &detail)
{
    if (str == "error-user-is-banned"_L1) {
        return i18n("User is banned from this room");
    }
    return RestApiAbstractJob::errorMessage(str, detail);
}

bool ChannelInviteJob::interceptError(const QJsonObject &replyObject)
{
    const QString errorType = replyObject["errorType"_L1].toString();
    if (errorType == "error-user-is-banned"_L1) {
        qDebug() << " bool ChannelInviteJob::interceptError(const QJsonObject &replyObject) banned !!!!";
        Q_EMIT needUnbanned(mChannelInviteInfo);
        return true;
    }
    return false;
}

bool RocketChatRestApi::ChannelInviteJob::ChannelInviteInfo::isValid() const
{
    return !identifier.isEmpty() && channelGroupInfoType != ChannelInviteJob::ChannelInviteInfoType::Unknown;
}

#include "moc_channelinvitejob.cpp"
