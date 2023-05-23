/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QString>

class LIBRUQOLACORE_EXPORT Role
{
public:
    Role();

    Q_REQUIRED_RESULT QString userId() const;
    void setUserId(const QString &userId);

    Q_REQUIRED_RESULT bool isModerator() const;
    void setIsModerator(bool isModerator);

    Q_REQUIRED_RESULT bool isLeader() const;
    void setIsLeader(bool isLeader);

    Q_REQUIRED_RESULT bool isOwner() const;
    void setIsOwner(bool isOwner);

    void parseRole(const QJsonObject &obj);

    Q_REQUIRED_RESULT bool operator==(const Role &other) const;

    Q_REQUIRED_RESULT bool isValid() const;
    void updateRole(const QString &str, bool b);
    Q_REQUIRED_RESULT bool hasARole() const;

    Q_REQUIRED_RESULT QString userName() const;
    void setUserName(const QString &newUserName);

private:
    QString mUserId;
    QString mUserName;
    bool mIsModerator = false;
    bool mIsLeader = false;
    bool mIsOwner = false;
};
Q_DECLARE_METATYPE(Role)
Q_DECLARE_TYPEINFO(Role, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Role &t);
