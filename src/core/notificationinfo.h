/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QJsonArray>
#include <QPixmap>

class LIBRUQOLACORE_EXPORT NotificationInfo
{
    Q_GADGET
public:
    NotificationInfo();
    ~NotificationInfo();

    enum class NotificationType : uint8_t {
        StandardMessage,
        ConferenceCall,
    };
    Q_ENUM(NotificationType);

    [[nodiscard]] const QString &accountName() const;
    void setAccountName(const QString &newAccountName);

    [[nodiscard]] const QString &message() const;
    void setMessage(const QString &newMessage);

    [[nodiscard]] const QString &title() const;
    void setTitle(const QString &newTitle);

    [[nodiscard]] const QByteArray &senderId() const;
    void setSenderId(const QByteArray &newSenderId);

    [[nodiscard]] const QString &senderName() const;
    void setSenderName(const QString &newSenderName);

    [[nodiscard]] const QString &senderUserName() const;
    void setSenderUserName(const QString &newSenderUserName);

    [[nodiscard]] const QString &roomName() const;
    void setRoomName(const QString &newRoomName);

    [[nodiscard]] const QByteArray &roomId() const;
    void setRoomId(const QByteArray &newRoomId);

    [[nodiscard]] const QString &channelType() const;
    void setChannelType(const QString &newChannelType);

    [[nodiscard]] const QByteArray &tmId() const;
    void setTmId(const QByteArray &newTmId);

    [[nodiscard]] const QPixmap &pixmap() const;
    void setPixmap(const QPixmap &newPixmap);

    void parseNotification(const QJsonArray &contents);

    // Pixmap can be null.
    [[nodiscard]] bool isValid() const;

    [[nodiscard]] QDateTime dateTime() const;
    void setDateTime(const QDateTime &newDateTime);

    [[nodiscard]] const QByteArray &messageId() const;
    void setMessageId(const QByteArray &newMessageId);

    [[nodiscard]] NotificationType notificationType() const;
    void setNotificationType(NotificationType newNotificationType);

    [[nodiscard]] bool operator==(const NotificationInfo &other) const;

private:
    QByteArray mMessageId;
    QString mAccountName;
    QString mMessage;
    QString mTitle;
    QByteArray mSenderId;
    QString mSenderName;
    QString mSenderUserName;
    QString mRoomName;
    QByteArray mRoomId;
    QString mChannelType;
    QByteArray mTmId;
    QDateTime mDateTime;
    QPixmap mPixmap;
    NotificationType mNotificationType = NotificationType::StandardMessage;
};
Q_DECLARE_TYPEINFO(NotificationInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const NotificationInfo &t);
