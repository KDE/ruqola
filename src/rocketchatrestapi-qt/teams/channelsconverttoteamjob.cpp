/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelsconverttoteamjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>


#include <KLocalizedString>

using namespace Qt::Literals::StringLiterals;
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

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ChannelsConvertToTeamJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT channelConvertToTeamDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ChannelsConvertToTeamJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
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
    jsonObj["channelId"_L1] = mChannelId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChannelsConvertToTeamJob::generateErrorMessage(const QString &errorStr) const
{
    if (errorStr == "unauthorized"_L1) {
        return i18n("Channel can not convert to Team.");
    } else if (errorStr == "team-name-already-exists"_L1) {
        return i18n("Team already exists.");
    }
    return RestApiAbstractJob::generateErrorMessage(errorStr);
}

#include "moc_channelsconverttoteamjob.cpp"
