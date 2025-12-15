/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "leavechanneljob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
LeaveChannelJob::LeaveChannelJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

LeaveChannelJob::~LeaveChannelJob() = default;

bool LeaveChannelJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("LeaveChannelJob::start");
    submitPostRequest(json());

    return true;
}

void LeaveChannelJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("LeaveChannelJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT leaveChannelDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("LeaveChannelJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool LeaveChannelJob::requireHttpAuthentication() const
{
    return true;
}

bool LeaveChannelJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "LeaveChannelJob: RoomId and RoomName are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument LeaveChannelJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest LeaveChannelJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsLeave);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_leavechanneljob.cpp"
