/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#ifndef ADMINROOMS_H
#define ADMINROOMS_H

#include "administrator/adminroom.h"
#include "libruqola_private_export.h"
#include <QVector>
#include <QDebug>
class DownloadAppsLanguagesManager;
class LIBRUQOLACORE_TESTS_EXPORT AdminRooms
{
public:
    AdminRooms();

    Q_REQUIRED_RESULT bool isEmpty() const;
    void clear();
    Q_REQUIRED_RESULT int count() const;
    Q_REQUIRED_RESULT AdminRoom at(int index) const;

    Q_REQUIRED_RESULT int offset() const;
    void setOffset(int offset);

    Q_REQUIRED_RESULT int total() const;
    void setTotal(int total);

    void parseAdminRooms(const QJsonObject &commandsObj);
    void parseMoreAdminRooms(const QJsonObject &fileAttachmentsObj);

    Q_REQUIRED_RESULT QVector<AdminRoom> adminRooms() const;
    void setAdminRooms(const QVector<AdminRoom> &commands);

    Q_REQUIRED_RESULT int adminRoomsCount() const;
    void setAdminRoomsCount(int adminroomsCount);

private:
    void parseListAdminRooms(const QJsonObject &commandsObj);
    QVector<AdminRoom> mAdminRooms;
    int mAdminRoomsCount = 0;
    int mOffset = 0;
    int mTotal = 0;
};

Q_DECLARE_METATYPE(AdminRooms)
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const AdminRooms &t);

#endif // ADMINROOMS_H
