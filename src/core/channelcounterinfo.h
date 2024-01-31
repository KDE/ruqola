/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

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

    [[nodiscard]] bool operator==(const ChannelCounterInfo &other) const;
    [[nodiscard]] bool operator!=(const ChannelCounterInfo &other) const;
    [[nodiscard]] quint64 unreadMessages() const;
    void setUnreadMessages(quint64 unreadMessages);

    [[nodiscard]] QDateTime unreadFrom() const;
    void setUnreadFrom(const QDateTime &unreadFrom);

    [[nodiscard]] quint64 messageCount() const;
    void setMessageCount(quint64 messageCount);

    [[nodiscard]] bool joined() const;
    void setJoined(bool joined);

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] qint64 unreadMessageTimeStep() const;
    void setUnreadMessageTimeStep(qint64 unreadMessageTimeStep);

private:
    QDateTime mUnreadFrom;
    quint64 mUnreadMessages = 0;
    quint64 mMessageCount = 0;
    qint64 mUnreadMessageTimeStep = 0;
    bool mJoined = false;
};
Q_DECLARE_METATYPE(ChannelCounterInfo)
Q_DECLARE_TYPEINFO(ChannelCounterInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ChannelCounterInfo &t);
