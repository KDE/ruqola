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

#include "managerdatapaths.h"
#include "ruqola_debug.h"
#include <QStandardPaths>

ManagerDataPaths::ManagerDataPaths()
{
    initializePaths();
}

ManagerDataPaths *ManagerDataPaths::self()
{
    static ManagerDataPaths s_self;
    return &s_self;
}

QString ManagerDataPaths::accountAvatarConfigPath(const QString &accountName) const
{
    const QString accountPath = accountConfigPath(accountName) + QStringLiteral("/avatar.conf");
    return accountPath;
}

QString ManagerDataPaths::accountConfigPath(const QString &accountName) const
{
    const QString accountPath = path(ManagerDataPaths::PathType::Config, accountName);
    return accountPath;
}

QString ManagerDataPaths::accountConfigFileName(const QString &accountName) const
{
    const QString accountPath = accountConfigPath(accountName) + QStringLiteral("/ruqola.conf");
    return accountPath;
}

QString ManagerDataPaths::path(ManagerDataPaths::PathType type, const QString &accountName, const QString &subdirectory) const
{
    QString path = mPathTypeHash.value(type);
    Q_ASSERT(!path.isEmpty());
    if (!accountName.isEmpty()) {
        path += QLatin1Char('/') + accountName;
    }
    switch (type) {
    case Picture:
    case Video:
        path += QStringLiteral("/Ruqola/recordings");
        break;
    case Config:
    case Cache:
        break;
    }
    if (!subdirectory.isEmpty()) {
        path += QLatin1Char('/') + subdirectory;
    }
    return path;
}

void ManagerDataPaths::initializePaths()
{
    mPathTypeHash.insert(PathType::Config, QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    mPathTypeHash.insert(PathType::Cache, QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    mPathTypeHash.insert(PathType::Video, QStandardPaths::writableLocation(QStandardPaths::MoviesLocation));
    mPathTypeHash.insert(PathType::Picture, QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    qCDebug(RUQOLA_LOG) << "mPathTypeHash:" << mPathTypeHash;
}
