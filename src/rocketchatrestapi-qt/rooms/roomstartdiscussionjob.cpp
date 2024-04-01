/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomstartdiscussionjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

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

    if (replyObject[QLatin1StringView("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("RoomStartDiscussionJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT startDiscussionDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
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
    jsonObj[QLatin1StringView("prid")] = QLatin1StringView(mParentRoomId);
    jsonObj[QLatin1StringView("t_name")] = mDiscussionName;
    if (!mParentMessageId.isEmpty()) {
        jsonObj[QLatin1StringView("pmid")] = QLatin1StringView(mParentMessageId);
    }
    if (!mReplyMessage.isEmpty()) {
        jsonObj[QLatin1StringView("reply")] = mReplyMessage;
    }
    if (!mUsers.isEmpty()) {
        QStringList lst;
        lst.reserve(mUsers.count());
        for (const QByteArray &b : mUsers) {
            lst.append(QLatin1StringView(b));
        }

        const QJsonArray usersJson = QJsonArray::fromStringList(lst);
        jsonObj[QLatin1StringView("users")] = usersJson;
    }
    // TODO add encrypted
    const QJsonDocument postData = QJsonDocument(jsonObj);
    // qDebug() << " postData " << postData;
    return postData;
}

QString RoomStartDiscussionJob::generateErrorMessage(const QString &errorStr) const
{
    if (errorStr == QLatin1StringView("error-invalid-room")) {
        return i18n("Invalid room");
    }
    return RestApiAbstractJob::generateErrorMessage(errorStr);
}

#include "moc_roomstartdiscussionjob.cpp"
