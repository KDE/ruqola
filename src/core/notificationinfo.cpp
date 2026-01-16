/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationinfo.h"
#include "ruqola_debug.h"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
NotificationInfo::NotificationInfo() = default;

NotificationInfo::~NotificationInfo() = default;

QDebug operator<<(QDebug d, const NotificationInfo &t)
{
    d.space() << "accountName" << t.accountName();
    d.space() << "message" << t.message();
    d.space() << "title" << t.title();
    d.space() << "sender" << t.senderId();
    d.space() << "senderName" << t.senderName();
    d.space() << "mSenderUserName" << t.senderUserName();
    d.space() << "roomName" << t.roomName();
    d.space() << "roomId" << t.roomId();
    d.space() << "type" << t.channelType();
    d.space() << "tmId" << t.tmId();
    d.space() << "pixmap is null ?" << t.pixmap().isNull();
    d.space() << "date time" << t.dateTime();
    d.space() << "messageId" << t.messageId();
    d.space() << "mNotificationType" << t.notificationType();
    d.space() << "forceShowAccountName" << t.forceShowAccountName();
    return d;
}

const QString &NotificationInfo::accountName() const
{
    return mAccountName;
}

void NotificationInfo::setAccountName(const QString &newAccountName)
{
    mAccountName = newAccountName;
}

const QString &NotificationInfo::message() const
{
    return mMessage;
}

void NotificationInfo::setMessage(const QString &newMessage)
{
    mMessage = newMessage;
}

const QString &NotificationInfo::title() const
{
    return mTitle;
}

void NotificationInfo::setTitle(const QString &newTitle)
{
    mTitle = newTitle;
}

const QByteArray &NotificationInfo::senderId() const
{
    return mSenderId;
}

void NotificationInfo::setSenderId(const QByteArray &newSenderId)
{
    mSenderId = newSenderId;
}

const QString &NotificationInfo::senderName() const
{
    return mSenderName;
}

void NotificationInfo::setSenderName(const QString &newSenderName)
{
    mSenderName = newSenderName;
}

const QString &NotificationInfo::senderUserName() const
{
    return mSenderUserName;
}

void NotificationInfo::setSenderUserName(const QString &newSenderUserName)
{
    mSenderUserName = newSenderUserName;
}

const QString &NotificationInfo::roomName() const
{
    return mRoomName;
}

void NotificationInfo::setRoomName(const QString &newRoomName)
{
    mRoomName = newRoomName;
}

const QByteArray &NotificationInfo::roomId() const
{
    return mRoomId;
}

void NotificationInfo::setRoomId(const QByteArray &newRoomId)
{
    mRoomId = newRoomId;
}

const QString &NotificationInfo::channelType() const
{
    return mChannelType;
}

void NotificationInfo::setChannelType(const QString &newChannelType)
{
    mChannelType = newChannelType;
}

const QByteArray &NotificationInfo::tmId() const
{
    return mTmId;
}

void NotificationInfo::setTmId(const QByteArray &newTmId)
{
    mTmId = newTmId;
}

const QPixmap &NotificationInfo::pixmap() const
{
    return mPixmap;
}

void NotificationInfo::setPixmap(const QPixmap &newPixmap)
{
    mPixmap = newPixmap;
}

void NotificationInfo::parseNotification(const QJsonArray &contents)
{
    // qDebug() << " NotificationInfo::parseNotification(const QJsonArray &contents)" << contents;
    const QJsonObject obj = contents.at(0).toObject();
    setTitle(obj["title"_L1].toString());
    const QJsonObject payloadObj = obj.value("payload"_L1).toObject();
    if (!payloadObj.isEmpty()) {
        setMessageId(payloadObj["_id"_L1].toString().toLatin1());
        setRoomId(payloadObj["rid"_L1].toString().toLatin1());
        setRoomName(payloadObj["name"_L1].toString());
        setChannelType(payloadObj["type"_L1].toString());
        setTmId(payloadObj["tmid"_L1].toString().toLatin1());
        const QJsonObject senderObj = payloadObj.value("sender"_L1).toObject();
        if (!senderObj.isEmpty()) {
            setSenderId(senderObj.value("_id"_L1).toString().toLatin1());
            setSenderName(senderObj.value("name"_L1).toString());
            setSenderUserName(senderObj.value("username"_L1).toString());
        } else {
            qCDebug(RUQOLA_LOG) << "Problem with notification json: missing sender";
        }
        const QJsonObject messageObj = payloadObj.value("message"_L1).toObject();
        if (messageObj.isEmpty()) {
            qCDebug(RUQOLA_LOG) << "Problem with notification json: missing message";
            setMessage(obj["text"_L1].toString());
        } else {
            if (messageObj["t"_L1].toString() == "videoconf"_L1) {
                mNotificationType = NotificationType::ConferenceCall;
            } else {
                setMessage(messageObj["msg"_L1].toString());
                if (message().isEmpty()) {
                    // Fallback to text
                    setMessage(obj["text"_L1].toString());
                }
            }
        }
    } else {
        qCDebug(RUQOLA_LOG) << "Problem with notification json: missing payload";
    }
    qCDebug(RUQOLA_LOG) << "info " << *this;
}

bool NotificationInfo::isValid() const
{
    bool valid = !mSenderId.isEmpty() && !mChannelType.isEmpty();
    if (mNotificationType == NotificationType::ConferenceCall) {
        return valid;
    }
    return valid && !mMessage.isEmpty();
}

QDateTime NotificationInfo::dateTime() const
{
    return mDateTime;
}

void NotificationInfo::setDateTime(const QDateTime &newDateTime)
{
    mDateTime = newDateTime;
}

const QByteArray &NotificationInfo::messageId() const
{
    return mMessageId;
}

void NotificationInfo::setMessageId(const QByteArray &newMessageId)
{
    mMessageId = newMessageId;
}

NotificationInfo::NotificationType NotificationInfo::notificationType() const
{
    return mNotificationType;
}

void NotificationInfo::setNotificationType(NotificationType newNotificationType)
{
    mNotificationType = newNotificationType;
}

bool NotificationInfo::operator==(const NotificationInfo &other) const
{
    return other.mMessageId == mMessageId && other.mAccountName == mAccountName && other.mMessage == mMessage && other.mTitle == mTitle
        && other.mSenderId == mSenderId && other.mSenderName == mSenderName && other.mSenderUserName == mSenderUserName && other.mRoomName == mRoomName
        && other.mRoomId == mRoomId && other.mChannelType == mChannelType && other.mTmId == mTmId && other.mDateTime == mDateTime
        && other.mPixmap.cacheKey() == mPixmap.cacheKey() && other.mNotificationType == mNotificationType
        && other.mForceShowAccountName == mForceShowAccountName;
}

bool NotificationInfo::forceShowAccountName() const
{
    return mForceShowAccountName;
}

void NotificationInfo::setForceShowAccountName(bool newForceShowAccountName)
{
    mForceShowAccountName = newForceShowAccountName;
}

#include "moc_notificationinfo.cpp"
