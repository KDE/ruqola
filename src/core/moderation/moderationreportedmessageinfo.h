/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QObject>
#include <QString>
class QJsonObject;
class LIBRUQOLACORE_EXPORT ModerationReportedMessageInfo
{
public:
    ModerationReportedMessageInfo();
    ~ModerationReportedMessageInfo() = default;

    [[nodiscard]] bool operator==(const ModerationReportedMessageInfo &other) const;

    void parseModerationInfo(const QJsonObject &replyObject);

    [[nodiscard]] QByteArray userId() const;
    void setUserId(const QByteArray &newUserId);

    [[nodiscard]] QString name() const;
    void setName(const QString &newName);

    [[nodiscard]] QString userName() const;
    void setUserName(const QString &newUserName);

    [[nodiscard]] QByteArray msgId() const;
    void setMsgId(const QByteArray &newMsgId);

    [[nodiscard]] int count() const;
    void setCount(int newCount);

    [[nodiscard]] bool isUserDeleted() const;
    void setIsUserDeleted(bool newIsUserDeleted);

    [[nodiscard]] QString message() const;
    void setMessage(const QString &newMessage);

    [[nodiscard]] const QString &createAtDisplayDateTime() const;

    [[nodiscard]] qint64 createdAt() const;
    void setCreatedAt(qint64 newCreatedAt);

    [[nodiscard]] QStringList roomList() const;
    void setRoomList(const QStringList &newRoomName);

private:
    LIBRUQOLACORE_NO_EXPORT void parseRoomList(const QJsonArray &rooms);
    QStringList mRoomList;
    QByteArray mUserId;
    QString mName;
    QString mUserName;
    QByteArray mMsgId;
    QString mMessage;
    QString mCreateAtDisplayDateTime;
    qint64 mCreatedAt = -1;
    int mCount = 0;
    bool mIsUserDeleted = false;
};

Q_DECLARE_METATYPE(ModerationReportedMessageInfo)
Q_DECLARE_TYPEINFO(ModerationReportedMessageInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportedMessageInfo &t);
