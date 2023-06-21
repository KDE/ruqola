/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "creategroupsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
CreateGroupsJob::CreateGroupsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

CreateGroupsJob::~CreateGroupsJob() = default;

bool CreateGroupsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("CreateGroupsJob::start");
    submitPostRequest(json());

    return true;
}

void CreateGroupsJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("CreateGroupsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT createGroupsDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("CreateGroupsJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

CreateChannelTeamInfo CreateGroupsJob::createGroupsInfo() const
{
    return mCreateGroupInfo;
}

void CreateGroupsJob::setCreateGroupsInfo(const CreateChannelTeamInfo &createGroupsInfo)
{
    mCreateGroupInfo = createGroupsInfo;
}

bool CreateGroupsJob::requireHttpAuthentication() const
{
    return true;
}

bool CreateGroupsJob::canStart() const
{
    if (!mCreateGroupInfo.canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "CreateGroupsJob: channelname is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument CreateGroupsJob::json() const
{
    return mCreateGroupInfo.json();
}

QNetworkRequest CreateGroupsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_creategroupsjob.cpp"
