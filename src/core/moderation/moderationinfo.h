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

    [[nodiscard]] bool operator==(const ModerationInfo &other) const;

    void parseModerationInfo(const QJsonObject &replyObject);

    [[nodiscard]] QString userId() const;
    void setUserId(const QString &newUserId);

    [[nodiscard]] QString name() const;
    void setName(const QString &newName);

    [[nodiscard]] QString userName() const;
    void setUserName(const QString &newUserName);

    [[nodiscard]] QString msgId() const;
    void setMsgId(const QString &newMsgId);

    [[nodiscard]] int count() const;
    void setCount(int newCount);

    [[nodiscard]] bool isUserDeleted() const;
    void setIsUserDeleted(bool newIsUserDeleted);

    [[nodiscard]] QString message() const;
    void setMessage(const QString &newMessage);

    [[nodiscard]] const QString &createAtDisplayDateTime() const;

    [[nodiscard]] qint64 createdAt() const;

    [[nodiscard]] QString roomName() const;
    void setRoomName(const QString &newRoomName);

private:
    LIBRUQOLACORE_NO_EXPORT void setCreatedAt(qint64 newCreatedAt);
    LIBRUQOLACORE_NO_EXPORT void parseRoom();
    qint64 mCreatedAt = -1;
    QString mUserId;
    QString mName;
    QString mUserName;
    QString mMsgId;
    QString mMessage;
    QString mCreateAtDisplayDateTime;
    QString mRoomName;
    int mCount = 0;
    bool mIsUserDeleted = false;
};

Q_DECLARE_METATYPE(ModerationInfo)
Q_DECLARE_TYPEINFO(ModerationInfo, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationInfo &t);
