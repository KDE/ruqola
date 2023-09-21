/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonArray>
#include <QPixmap>

class LIBRUQOLACORE_EXPORT NotificationInfo
{
    Q_GADGET
public:
    NotificationInfo();
    ~NotificationInfo();

    enum NotificationType {
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

    [[nodiscard]] const QString &senderId() const;
    void setSenderId(const QString &newSenderId);

    [[nodiscard]] const QString &senderName() const;
    void setSenderName(const QString &newSenderName);

    [[nodiscard]] const QString &senderUserName() const;
    void setSenderUserName(const QString &newSenderUserName);

    [[nodiscard]] const QString &roomName() const;
    void setRoomName(const QString &newRoomName);

    [[nodiscard]] const QString &roomId() const;
    void setRoomId(const QString &newRoomId);

    [[nodiscard]] const QString &channelType() const;
    void setChannelType(const QString &newChannelType);

    [[nodiscard]] const QString &tmId() const;
    void setTmId(const QString &newTmId);

    [[nodiscard]] const QPixmap &pixmap() const;
    void setPixmap(const QPixmap &newPixmap);

    void parseNotification(const QJsonArray &contents);

    // Pixmap can be null.
    [[nodiscard]] bool isValid() const;

    [[nodiscard]] const QString &dateTime() const;
    void setDateTime(const QString &newDateTime);

    [[nodiscard]] const QString &messageId() const;
    void setMessageId(const QString &newMessageId);

    [[nodiscard]] NotificationType notificationType() const;
    void setNotificationType(const NotificationType &newNotificationType);

    [[nodiscard]] bool operator==(const NotificationInfo &other) const;

private:
    QString mMessageId;
    QString mAccountName;
    QString mMessage;
    QString mTitle;
    QString mSenderId;
    QString mSenderName;
    QString mSenderUserName;
    QString mRoomName;
    QString mRoomId;
    QString mChannelType;
    QString mTmId;
    QString mDateTime;
    QPixmap mPixmap;
    NotificationType mNotificationType = StandardMessage;
};
Q_DECLARE_TYPEINFO(NotificationInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const NotificationInfo &t);
