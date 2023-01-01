/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
// TODO rename it
class LIBRUQOLACORE_EXPORT Channel
{
    Q_GADGET
public:
    enum ChannelType {
        Room,
        DirectChannel,
        Unknown,
    };
    Q_ENUM(ChannelType)

    Channel();
    ~Channel();
    void parseChannel(const QJsonObject &object, Channel::ChannelType);

    Q_REQUIRED_RESULT ChannelType type() const;
    void setType(Channel::ChannelType type);

    Q_REQUIRED_RESULT bool operator==(const Channel &other) const;

    Channel &operator=(const Channel &other) = default;

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &roomId);

    Q_REQUIRED_RESULT QString roomName() const;
    void setRoomName(const QString &roomName);

    Q_REQUIRED_RESULT QString roomType() const;
    void setRoomType(const QString &roomType);

    Q_REQUIRED_RESULT QString userId() const;
    void setUserId(const QString &userId);

    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &name);

    Q_REQUIRED_RESULT QString status() const;
    void setStatus(const QString &status);

    Q_REQUIRED_RESULT QString userName() const;
    void setUserName(const QString &userName);

    Q_REQUIRED_RESULT QString iconFromStatus() const;

private:
    QString mRoomId;
    QString mRoomName;
    QString mRoomType;

    // Private Channel
    QString mUserId;
    QString mName;
    QString mStatus;
    QString mUserName;
    ChannelType mType = ChannelType::Unknown;
};
Q_DECLARE_METATYPE(Channel)
Q_DECLARE_TYPEINFO(Channel, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Channel &t);
