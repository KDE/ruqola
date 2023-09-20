/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT ModerationInfo
{
public:
    ModerationInfo();
    ~ModerationInfo() = default;

    Q_REQUIRED_RESULT bool operator==(const ModerationInfo &other) const;

    void parseModerationInfo(const QJsonObject &replyObject);

    Q_REQUIRED_RESULT QString userId() const;
    void setUserId(const QString &newUserId);

    Q_REQUIRED_RESULT QString name() const;
    void setName(const QString &newName);

    Q_REQUIRED_RESULT QString userName() const;
    void setUserName(const QString &newUserName);

    Q_REQUIRED_RESULT QString msgId() const;
    void setMsgId(const QString &newMsgId);

    Q_REQUIRED_RESULT int count() const;
    void setCount(int newCount);

    Q_REQUIRED_RESULT bool isUserDeleted() const;
    void setIsUserDeleted(bool newIsUserDeleted);

    Q_REQUIRED_RESULT QString message() const;
    void setMessage(const QString &newMessage);

private:
    QString mUserId;
    QString mName;
    QString mUserName;
    QString mMsgId;
    QString mMessage;
    int mCount = 0;
    bool mIsUserDeleted = false;
};

Q_DECLARE_METATYPE(ModerationInfo)
Q_DECLARE_TYPEINFO(ModerationInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationInfo &t);
