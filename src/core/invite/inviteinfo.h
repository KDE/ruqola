/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDateTime>
#include <QDebug>

class LIBRUQOLACORE_EXPORT InviteInfo
{
public:
    InviteInfo();
    ~InviteInfo() = default;
    void parseInviteInfo(const QJsonObject &replyObject);
    const QString &identifier() const;
    void setIdentifier(const QString &newIdentifier);

    [[nodiscard]] int maxUses() const;
    void setMaxUses(int newMaxUses);

    [[nodiscard]] const QString &userIdentifier() const;
    void setUserIdentifier(const QString &newUserIdentifier);

    [[nodiscard]] const QString &roomId() const;
    void setRoomId(const QString &newRoomId);

    [[nodiscard]] int uses() const;
    void setUses(int newUses);

    [[nodiscard]] const QDateTime &expireDateTime() const;
    void setExpireDateTime(const QDateTime &newExpireDateTime);

    [[nodiscard]] const QDateTime &createDateTime() const;
    void setCreateDateTime(const QDateTime &newCreateDateTime);

    [[nodiscard]] bool operator==(const InviteInfo &other) const;

private:
    QString mUserIdentifier;
    QString mIdentifier;
    QString mRoomId;
    QDateTime mExpireDateTime;
    QDateTime mCreateDateTime;
    int mUses = 0;
    int mMaxUses = 0;
};

Q_DECLARE_METATYPE(InviteInfo)
Q_DECLARE_TYPEINFO(InviteInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const InviteInfo &t);
