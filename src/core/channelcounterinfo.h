/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDateTime>
#include <QMetaType>
class QJsonObject;

class LIBRUQOLACORE_EXPORT ChannelCounterInfo : public QSharedData
{
public:
    explicit ChannelCounterInfo(const ChannelCounterInfo &other);
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
QT_DECL_METATYPE_EXTERN_TAGGED(ChannelCounterInfo, Ruqola_ChannelCounterInfo, LIBRUQOLACORE_EXPORT)
Q_DECLARE_TYPEINFO(ChannelCounterInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ChannelCounterInfo &t);
