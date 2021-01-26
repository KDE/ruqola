/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#ifndef CHANNELCOUNTERINFO_H
#define CHANNELCOUNTERINFO_H

#include "libruqolacore_export.h"
#include <QDateTime>
#include <QDebug>
#include <QJsonObject>
class LIBRUQOLACORE_EXPORT ChannelCounterInfo
{
public:
    ChannelCounterInfo();
    ~ChannelCounterInfo();
    void parseCounterInfo(const QJsonObject &replyObject);

    Q_REQUIRED_RESULT bool operator==(const ChannelCounterInfo &other) const;
    Q_REQUIRED_RESULT bool operator!=(const ChannelCounterInfo &other) const;
    Q_REQUIRED_RESULT quint64 unreadMessages() const;
    void setUnreadMessages(quint64 unreadMessages);

    Q_REQUIRED_RESULT QDateTime unreadFrom() const;
    void setUnreadFrom(const QDateTime &unreadFrom);

    Q_REQUIRED_RESULT quint64 messageCount() const;
    void setMessageCount(quint64 messageCount);

    Q_REQUIRED_RESULT bool joined() const;
    void setJoined(bool joined);

    Q_REQUIRED_RESULT bool isValid() const;

private:
    QDateTime mUnreadFrom;
    quint64 mUnreadMessages = 0;
    quint64 mMessageCount = 0;
    bool mJoined = false;
};
Q_DECLARE_METATYPE(ChannelCounterInfo)
Q_DECLARE_TYPEINFO(ChannelCounterInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ChannelCounterInfo &t);
#endif // CHANNELCOUNTERINFO_H
