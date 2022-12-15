/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

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

const QString &NotificationInfo::senderId() const
{
    return mSenderId;
}

void NotificationInfo::setSenderId(const QString &newSenderId)
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

const QString &NotificationInfo::roomId() const
{
    return mRoomId;
}

void NotificationInfo::setRoomId(const QString &newRoomId)
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

const QString &NotificationInfo::tmId() const
{
    return mTmId;
}

void NotificationInfo::setTmId(const QString &newTmId)
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
    setTitle(obj[QStringLiteral("title")].toString());
    const QJsonObject payloadObj = obj.value(QLatin1String("payload")).toObject();
    if (!payloadObj.isEmpty()) {
        setMessageId(payloadObj[QStringLiteral("_id")].toString());
        setRoomId(payloadObj[QStringLiteral("rid")].toString());
        setRoomName(payloadObj[QStringLiteral("name")].toString());
        setChannelType(payloadObj[QStringLiteral("type")].toString());
        setTmId(payloadObj[QStringLiteral("tmid")].toString());
        const QJsonObject senderObj = payloadObj.value(QLatin1String("sender")).toObject();
        if (!senderObj.isEmpty()) {
            setSenderId(senderObj.value(QLatin1String("_id")).toString());
            setSenderName(senderObj.value(QLatin1String("name")).toString());
            setSenderUserName(senderObj.value(QLatin1String("username")).toString());
        } else {
            qCDebug(RUQOLA_LOG) << "Problem with notification json: missing sender";
        }
        const QJsonObject messageObj = payloadObj.value(QLatin1String("message")).toObject();
        if (messageObj.isEmpty()) {
            qCDebug(RUQOLA_LOG) << "Problem with notification json: missing message";
            setMessage(obj[QStringLiteral("text")].toString());
        } else {
            if (messageObj[QStringLiteral("t")].toString() == QLatin1String("videoconf")) {
                mNotificationType = ConferenceCall;
            } else {
                setMessage(messageObj[QStringLiteral("msg")].toString());
                if (message().isEmpty()) {
                    // Fallback to text
                    setMessage(obj[QStringLiteral("text")].toString());
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

const QString &NotificationInfo::messageId() const
{
    return mMessageId;
}

void NotificationInfo::setMessageId(const QString &newMessageId)
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
