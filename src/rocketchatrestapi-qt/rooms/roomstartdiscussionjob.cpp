/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomstartdiscussionjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
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

void RoomStartDiscussionJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoomStartDiscussionJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT startDiscussionDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoomStartDiscussionJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool RoomStartDiscussionJob::encrypted() const
{
    return mEncrypted;
}

void RoomStartDiscussionJob::setEncrypted(bool newEncrypted)
{
    mEncrypted = newEncrypted;
}

QString RoomStartDiscussionJob::replyMessage() const
{
    return mReplyMessage;
}

void RoomStartDiscussionJob::setReplyMessage(const QString &reply)
{
    mReplyMessage = reply;
}

QList<QByteArray> RoomStartDiscussionJob::users() const
{
    return mUsers;
}

void RoomStartDiscussionJob::setUsers(const QList<QByteArray> &value)
{
    mUsers = value;
}

QByteArray RoomStartDiscussionJob::parentMessageId() const
{
    return mParentMessageId;
}

void RoomStartDiscussionJob::setParentMessageId(const QByteArray &parentMessageId)
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

QByteArray RoomStartDiscussionJob::parentRoomId() const
{
    return mParentRoomId;
}

void RoomStartDiscussionJob::setParentRoomId(const QByteArray &parentId)
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
    jsonObj["prid"_L1] = QLatin1StringView(mParentRoomId);
    jsonObj["t_name"_L1] = mDiscussionName;
    if (!mParentMessageId.isEmpty()) {
        jsonObj["pmid"_L1] = QLatin1StringView(mParentMessageId);
    }
    if (!mReplyMessage.isEmpty()) {
        jsonObj["reply"_L1] = mReplyMessage;
    }
    if (!mUsers.isEmpty()) {
        QStringList lst;
        lst.reserve(mUsers.count());
        for (const QByteArray &b : mUsers) {
            lst.append(QLatin1StringView(b));
        }

        const QJsonArray usersJson = QJsonArray::fromStringList(lst);
        jsonObj["users"_L1] = usersJson;
    }
    if (mEncrypted) {
        jsonObj["encrypted"_L1] = mEncrypted;
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    // qDebug() << " postData " << postData;
    return postData;
}

QString RoomStartDiscussionJob::generateErrorMessage(const QString &errorStr) const
{
    if (errorStr == "error-invalid-room"_L1) {
        return i18n("Invalid room");
    }
    return RestApiAbstractJob::generateErrorMessage(errorStr);
}

#include "moc_roomstartdiscussionjob.cpp"
