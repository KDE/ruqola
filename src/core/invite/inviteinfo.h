/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDateTime>
#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT InviteInfo
{
public:
    InviteInfo();
    ~InviteInfo() = default;
    void parseInviteInfo(const QJsonObject &replyObject);
    const QString &identifier() const;
    void setIdentifier(const QString &newIdentifier);

    int maxUses() const;
    void setMaxUses(int newMaxUses);

    const QString &userIdentifier() const;
    void setUserIdentifier(const QString &newUserIdentifier);

    const QString &roomId() const;
    void setRoomId(const QString &newRoomId);

    int uses() const;
    void setUses(int newUses);

    const QDateTime &expireDateTime() const;
    void setExpireDateTime(const QDateTime &newExpireDateTime);

    const QDateTime &createDateTime() const;
    void setCreateDateTime(const QDateTime &newCreateDateTime);

    Q_REQUIRED_RESULT bool operator==(const InviteInfo &other) const;

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
Q_DECLARE_TYPEINFO(InviteInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const InviteInfo &t);
