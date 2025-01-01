/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_EXPORT ModerationReportedUserInfo
{
public:
    ModerationReportedUserInfo();
    ~ModerationReportedUserInfo() = default;
    [[nodiscard]] bool operator==(const ModerationReportedUserInfo &other) const;

    void parseModerationReportedUserInfo(const QJsonObject &replyObject);

    [[nodiscard]] qint64 createdAt() const;
    void setCreatedAt(qint64 newTimeStamp);

    [[nodiscard]] int count() const;
    void setCount(int newCount);

    [[nodiscard]] const QString &createAtDisplayDateTime() const;

    [[nodiscard]] QString username() const;
    void setUsername(const QString &newUsername);

    [[nodiscard]] QString name() const;
    void setName(const QString &newName);

    [[nodiscard]] QByteArray identifier() const;
    void setIdentifier(const QByteArray &newIdentifier);

private:
    LIBRUQOLACORE_NO_EXPORT void parseReportedUser(const QJsonObject &o);
    QString mUsername;
    QString mName;
    QString mCreateAtDisplayDateTime;
    QByteArray mIdentifier;
    qint64 mCreatedAt = -1;
    int mCount = 0;
};

Q_DECLARE_METATYPE(ModerationReportedUserInfo)
Q_DECLARE_TYPEINFO(ModerationReportedUserInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationReportedUserInfo &t);
