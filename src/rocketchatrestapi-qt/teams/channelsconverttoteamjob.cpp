/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelsconverttoteamjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

#include <KLocalizedString>

using namespace RocketChatRestApi;
ChannelsConvertToTeamJob::ChannelsConvertToTeamJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelsConvertToTeamJob::~ChannelsConvertToTeamJob() = default;

bool ChannelsConvertToTeamJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelsConvertToTeamJob::start");
    submitPostRequest(json());

    return true;
}

void ChannelsConvertToTeamJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ChannelsConvertToTeamJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT channelConvertToTeamDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ChannelsConvertToTeamJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ChannelsConvertToTeamJob::channelId() const
{
    return mChannelId;
}

void ChannelsConvertToTeamJob::setChannelId(const QString &channelId)
{
    mChannelId = channelId;
}

bool ChannelsConvertToTeamJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelsConvertToTeamJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mChannelId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelsConvertToTeamJob: mTeamId is empty";
        return false;
    }
    return true;
}

QNetworkRequest ChannelsConvertToTeamJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsConvertToTeam);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ChannelsConvertToTeamJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("channelId")] = mChannelId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChannelsConvertToTeamJob::generateErrorMessage(const QString &errorStr) const
{
    if (errorStr == QLatin1String("unauthorized")) {
        return i18n("Channel can not convert to Team.");
    } else if (errorStr == QLatin1String("team-name-already-exists")) {
        return i18n("Team already exists.");
    }
    return RestApiAbstractJob::generateErrorMessage(errorStr);
}

#include "moc_channelsconverttoteamjob.cpp"
