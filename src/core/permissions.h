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
#include "permission.h"
#include <QJsonObject>
#include <QVector>

class LIBRUQOLACORE_TESTS_EXPORT Permissions
{
public:
    Permissions();

    Q_REQUIRED_RESULT QVector<Permission> permissions() const;
    void setPermissions(const QVector<Permission> &permissions);

    void parsePermissions(const QJsonObject &obj);
    Q_REQUIRED_RESULT bool isEmpty() const;

    Q_REQUIRED_RESULT int count() const;
    void clear();

    Q_REQUIRED_RESULT Permission at(int index) const;

private:
    QVector<Permission> mPermissions;
};
