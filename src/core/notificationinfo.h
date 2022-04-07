/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonArray>
#include <QPixmap>

class LIBRUQOLACORE_EXPORT NotificationInfo
{
public:
    NotificationInfo();
    ~NotificationInfo() = default;

    Q_REQUIRED_RESULT const QString &accountName() const;
    void setAccountName(const QString &newAccountName);

    Q_REQUIRED_RESULT const QString &message() const;
    void setMessage(const QString &newMessage);

    Q_REQUIRED_RESULT const QString &title() const;
    void setTitle(const QString &newTitle);

    Q_REQUIRED_RESULT const QString &senderId() const;
    void setSenderId(const QString &newSenderId);

    Q_REQUIRED_RESULT const QString &senderName() const;
    void setSenderName(const QString &newSenderName);

    Q_REQUIRED_RESULT const QString &senderUserName() const;
    void setSenderUserName(const QString &newSenderUserName);

    Q_REQUIRED_RESULT const QString &roomName() const;
    void setRoomName(const QString &newRoomName);

    Q_REQUIRED_RESULT const QString &roomId() const;
    void setRoomId(const QString &newRoomId);

    Q_REQUIRED_RESULT const QString &channelType() const;
    void setChannelType(const QString &newChannelType);

    Q_REQUIRED_RESULT const QString &tmId() const;
    void setTmId(const QString &newTmId);

    Q_REQUIRED_RESULT const QPixmap &pixmap() const;
    void setPixmap(const QPixmap &newPixmap);

    void parseNotification(const QJsonArray &contents);

    // Pixmap can be null.
    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT const QDateTime &dateTime() const;
    void setDateTime(const QDateTime &newDateTime);

private:
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
    QPixmap mPixmap;
    QDateTime mDateTime;
};
Q_DECLARE_TYPEINFO(NotificationInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const NotificationInfo &t);
