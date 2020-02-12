/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "roomstartdiscussionjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
RoomStartDiscussionJob::RoomStartDiscussionJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomStartDiscussionJob::~RoomStartDiscussionJob()
{
}

bool RoomStartDiscussionJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("RoomStartDiscussionJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &RoomStartDiscussionJob::slotStartDiscussionFinished);
    return true;
}

void RoomStartDiscussionJob::slotStartDiscussionFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("RoomStartDiscussionJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT startDiscussionDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("RoomStartDiscussionJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
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
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start RoomStartDiscussionJob";
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
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
