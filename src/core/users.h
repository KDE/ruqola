/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#pragma once

#include "libruqola_private_export.h"
#include "user.h"
#include <QDebug>
#include <QVector>
class LIBRUQOLACORE_EXPORT Users
{
public:
    enum ParseType {
        Administrator,
        Directory,
    };
    Users();

    Q_REQUIRED_RESULT bool isEmpty() const;
    void clear();
    Q_REQUIRED_RESULT int count() const;
    Q_REQUIRED_RESULT User at(int index) const;

    Q_REQUIRED_RESULT User &operator[](int i);

    Q_REQUIRED_RESULT int offset() const;
    void setOffset(int offset);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int total);

    User takeAt(int index);

    void parseUsers(const QJsonObject &obj, ParseType type);
    void parseMoreUsers(const QJsonObject &obj, ParseType type);

    Q_REQUIRED_RESULT QVector<User> users() const;
    void setUsers(const QVector<User> &rooms);

    Q_REQUIRED_RESULT int roomsCount() const;
    void setRoomsCount(int adminroomsCount);

private:
    void parseListUsers(const QJsonObject &obj, ParseType type);
    QVector<User> mUsers;
    int mUsersCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};

Q_DECLARE_METATYPE(Users)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Users &t);
