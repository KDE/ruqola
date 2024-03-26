/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationinfo.h"
#include "ruqola_debug.h"
#include <QJsonObject>

NotificationInfo::NotificationInfo() = default;

NotificationInfo::~NotificationInfo() = default;

QDebug operator<<(QDebug d, const NotificationInfo &t)
{
    d << " accountName " << t.accountName();
    d << " message " << t.message();
    d << " title " << t.title();
    d << " sender " << t.senderId();
    d << " senderName " << t.senderName();
    d << " mSenderUserName " << t.senderUserName();
    d << " roomName " << t.roomName();
    d << " roomId " << t.roomId();
    d << " type " << t.channelType();
    d << " tmId " << t.tmId();
    d << " pixmap is null ? " << t.pixmap().isNull();
    d << " date time " << t.dateTime();
    d << " messageId " << t.messageId();
    d << " mNotificationType " << t.notificationType();
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
    setTitle(obj[QLatin1StringView("title")].toString());
    const QJsonObject payloadObj = obj.value(QLatin1StringView("payload")).toObject();
    if (!payloadObj.isEmpty()) {
        setMessageId(payloadObj[QLatin1StringView("_id")].toString().toLatin1());
        setRoomId(payloadObj[QLatin1StringView("rid")].toString().toLatin1());
        setRoomName(payloadObj[QLatin1StringView("name")].toString());
        setChannelType(payloadObj[QLatin1StringView("type")].toString());
        setTmId(payloadObj[QLatin1StringView("tmid")].toString().toLatin1());
        const QJsonObject senderObj = payloadObj.value(QLatin1StringView("sender")).toObject();
        if (!senderObj.isEmpty()) {
            setSenderId(senderObj.value(QLatin1StringView("_id")).toString().toLatin1());
            setSenderName(senderObj.value(QLatin1StringView("name")).toString());
            setSenderUserName(senderObj.value(QLatin1StringView("username")).toString());
        } else {
            qCDebug(RUQOLA_LOG) << "Problem with notification json: missing sender";
        }
        const QJsonObject messageObj = payloadObj.value(QLatin1StringView("message")).toObject();
        if (messageObj.isEmpty()) {
            qCDebug(RUQOLA_LOG) << "Problem with notification json: missing message";
            setMessage(obj[QLatin1StringView("text")].toString());
        } else {
            if (messageObj[QLatin1StringView("t")].toString() == QLatin1StringView("videoconf")) {
                mNotificationType = ConferenceCall;
            } else {
                setMessage(messageObj[QLatin1StringView("msg")].toString());
                if (message().isEmpty()) {
                    // Fallback to text
                    setMessage(obj[QLatin1StringView("text")].toString());
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
    if (mNotificationType == ConferenceCall) {
        return valid;
    }
    return valid && !mMessage.isEmpty();
}

const QString &NotificationInfo::dateTime() const
{
    return mDateTime;
}

void NotificationInfo::setDateTime(const QString &newDateTime)
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

void NotificationInfo::setNotificationType(const NotificationType &newNotificationType)
{
    mNotificationType = newNotificationType;
}

bool NotificationInfo::operator==(const NotificationInfo &other) const
{
    return other.mMessageId == mMessageId && other.mAccountName == mAccountName && other.mMessage == mMessage && other.mTitle == mTitle
        && other.mSenderId == mSenderId && other.mSenderName == mSenderName && other.mSenderUserName == mSenderUserName && other.mRoomName == mRoomName
        && other.mRoomId == mRoomId && other.mChannelType == mChannelType && other.mTmId == mTmId && other.mDateTime == mDateTime
        && other.mPixmap.cacheKey() == mPixmap.cacheKey() && other.mNotificationType == mNotificationType;
}

#include "moc_notificationinfo.cpp"
