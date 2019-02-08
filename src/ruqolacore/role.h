/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef ROLE_H
#define ROLE_H
#include "libruqola_private_export.h"
#include <QString>
#include <QDebug>
#include <QJsonObject>

class LIBRUQOLACORE_TESTS_EXPORT Role
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

     Q_REQUIRED_RESULT bool operator ==(const Role &other) const;

     Q_REQUIRED_RESULT bool isValid() const;
     void updateRole(const QString &str, bool b);
     Q_REQUIRED_RESULT bool hasARole() const;
private:
    QString mUserId;
    bool mIsModerator = false;
    bool mIsLeader = false;
    bool mIsOwner = false;
};
Q_DECLARE_METATYPE(Role)
Q_DECLARE_TYPEINFO(Role, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Role &t);
#endif // ROLE_H
