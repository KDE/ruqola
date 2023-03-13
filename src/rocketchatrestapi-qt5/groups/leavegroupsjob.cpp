/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "leavegroupsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
LeaveGroupsJob::LeaveGroupsJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

LeaveGroupsJob::~LeaveGroupsJob() = default;

bool LeaveGroupsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("LeaveGroupsJob::start");
    submitPostRequest(json());

    return true;
}

void LeaveGroupsJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("LeaveGroupsJob: Success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT leaveGroupsDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("LeaveGroupsJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool LeaveGroupsJob::requireHttpAuthentication() const
{
    return true;
}

bool LeaveGroupsJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "LeaveGroupsJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument LeaveGroupsJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest LeaveGroupsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsLeave);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
