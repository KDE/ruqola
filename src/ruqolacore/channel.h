/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#ifndef CHANNEL_H
#define CHANNEL_H

#include "libruqola_private_export.h"
#include <QDebug>
//TODO rename it
class LIBRUQOLACORE_TESTS_EXPORT Channel
{
    Q_GADGET
public:
    enum ChannelType {
        Room,
        PrivateChannel,
        Unknown
    };
    Q_ENUM(ChannelType)

    Channel();
    ~Channel();
    void parseChannel(const QJsonObject &object, Channel::ChannelType);

    Q_REQUIRED_RESULT ChannelType type() const;
    void setType(const ChannelType &type);

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

    //Private Channel
    QString mUserId;
    QString mName;
    QString mStatus;
    QString mUserName;
    ChannelType mType = ChannelType::Unknown;
};
Q_DECLARE_METATYPE(Channel)
Q_DECLARE_TYPEINFO(Channel, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Channel &t);

#endif // CHANNEL_H
