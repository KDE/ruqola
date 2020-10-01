/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#ifndef MANAGERDATAPATHS_H
#define MANAGERDATAPATHS_H

#include <QHash>
#include <QString>
#include "libruqola_private_export.h"

class LIBRUQOLACORE_EXPORT ManagerDataPaths
{
public:
    enum PathType {
        Config = 0,
        Cache = 1,
        Picture = 2,
        Video = 3,
    };
    ManagerDataPaths();
    static ManagerDataPaths *self();

    Q_REQUIRED_RESULT QString path(PathType type, const QString &accountName, const QString &subdirectory = QString()) const;

    Q_REQUIRED_RESULT QString accountConfigFileName(const QString &accountName) const;
    Q_REQUIRED_RESULT QString accountAvatarConfigPath(const QString &accountName) const;
private:
    void initializePaths();
    QHash<ManagerDataPaths::PathType, QString> mPathTypeHash;
    Q_REQUIRED_RESULT QString accountConfigPath(const QString &accountName) const;
};
#endif // MANAGERDATAPATHS_H
