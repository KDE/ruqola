/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDateTime>

class LIBRUQOLACORE_EXPORT InviteInfo
{
public:
    InviteInfo();
    ~InviteInfo() = default;
    void parseInviteInfo(const QJsonObject &replyObject);
    [[nodiscard]] const QByteArray &identifier() const;
    void setIdentifier(const QByteArray &newIdentifier);

    [[nodiscard]] int maxUses() const;
    void setMaxUses(int newMaxUses);

    [[nodiscard]] const QByteArray &userIdentifier() const;
    void setUserIdentifier(const QByteArray &newUserIdentifier);

    [[nodiscard]] const QByteArray &roomId() const;
    void setRoomId(const QByteArray &newRoomId);

    [[nodiscard]] int uses() const;
    void setUses(int newUses);

    [[nodiscard]] const QDateTime &expireDateTime() const;
    void setExpireDateTime(const QDateTime &newExpireDateTime);

    [[nodiscard]] const QDateTime &createDateTime() const;
    void setCreateDateTime(const QDateTime &newCreateDateTime);

    [[nodiscard]] bool operator==(const InviteInfo &other) const;

private:
    QByteArray mUserIdentifier;
    QByteArray mIdentifier;
    QByteArray mRoomId;
    QDateTime mExpireDateTime;
    QDateTime mCreateDateTime;
    int mUses = 0;
    int mMaxUses = 0;
};

Q_DECLARE_METATYPE(InviteInfo)
Q_DECLARE_TYPEINFO(InviteInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const InviteInfo &t);
