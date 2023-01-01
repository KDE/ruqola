/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomstartdiscussionjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

using namespace RocketChatRestApi;
RoomStartDiscussionJob::RoomStartDiscussionJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomStartDiscussionJob::~RoomStartDiscussionJob() = default;

bool RoomStartDiscussionJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoomStartDiscussionJob::start");
    submitPostRequest(json());

    return true;
}

void RoomStartDiscussionJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("RoomStartDiscussionJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT startDiscussionDone();
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("RoomStartDiscussionJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString RoomStartDiscussionJob::replyMessage() const
{
    return mReplyMessage;
}

void RoomStartDiscussionJob::setReplyMessage(const QString &reply)
{
    mReplyMessage = reply;
}

QStringList RoomStartDiscussionJob::users() const
{
    return mUsers;
}

void RoomStartDiscussionJob::setUsers(const QStringList &value)
{
    mUsers = value;
}

QString RoomStartDiscussionJob::parentMessageId() const
{
    return mParentMessageId;
}

void RoomStartDiscussionJob::setParentMessageId(const QString &parentMessageId)
{
    mParentMessageId = parentMessageId;
}

QString RoomStartDiscussionJob::discussionName() const
{
    return mDiscussionName;
}

void RoomStartDiscussionJob::setDiscussionName(const QString &discussionName)
{
    mDiscussionName = discussionName;
}

QString RoomStartDiscussionJob::parentRoomId() const
{
    return mParentRoomId;
}

void RoomStartDiscussionJob::setParentRoomId(const QString &parentId)
{
    mParentRoomId = parentId;
}

bool RoomStartDiscussionJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomStartDiscussionJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mParentRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomStartDiscussionJob: mParentId is empty";
        return false;
    }
    if (mDiscussionName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomStartDiscussionJob: mDiscussionName is empty";
        return false;
    }
    return true;
}

QNetworkRequest RoomStartDiscussionJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsCreateDiscussion);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument RoomStartDiscussionJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("prid")] = mParentRoomId;
    jsonObj[QLatin1String("t_name")] = mDiscussionName;
    if (!mParentMessageId.isEmpty()) {
        jsonObj[QLatin1String("pmid")] = mParentMessageId;
    }
    if (!mReplyMessage.isEmpty()) {
        jsonObj[QLatin1String("reply")] = mReplyMessage;
    }
    if (!mUsers.isEmpty()) {
        const QJsonArray usersJson = QJsonArray::fromStringList(mUsers);
        jsonObj[QLatin1String("users")] = usersJson;
    }
    // TODO add encrypted
    const QJsonDocument postData = QJsonDocument(jsonObj);
    // qDebug() << " postData " << postData;
    return postData;
}

QString RoomStartDiscussionJob::generateErrorMessage(const QString &errorStr) const
{
    if (errorStr == QLatin1String("error-invalid-room")) {
        return i18n("Invalid room");
    }
    return RestApiAbstractJob::generateErrorMessage(errorStr);
}
