/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QDebug>
#include <QString>

class LIBRUQOLACORE_EXPORT Role
{
public:
    Role();

    [[nodiscard]] QString userId() const;
    void setUserId(const QString &userId);

    [[nodiscard]] bool isModerator() const;
    void setIsModerator(bool isModerator);

    [[nodiscard]] bool isLeader() const;
    void setIsLeader(bool isLeader);

    [[nodiscard]] bool isOwner() const;
    void setIsOwner(bool isOwner);

    void parseRole(const QJsonObject &obj);

    [[nodiscard]] bool operator==(const Role &other) const;

    [[nodiscard]] bool isValid() const;
    void updateRole(const QString &str, bool b);
    [[nodiscard]] bool hasARole() const;

    [[nodiscard]] QString userName() const;
    void setUserName(const QString &newUserName);

private:
    QString mUserId;
    QString mUserName;
    bool mIsModerator = false;
    bool mIsLeader = false;
    bool mIsOwner = false;
};
Q_DECLARE_METATYPE(Role)
Q_DECLARE_TYPEINFO(Role, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Role &t);
