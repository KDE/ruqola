/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "managerdatapaths.h"

#include "ruqola_debug.h"
#include <QStandardPaths>

using namespace Qt::Literals::StringLiterals;
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
    const QString accountPath = accountConfigPath(accountName) + u"/avatar.conf"_s;
    return accountPath;
}

QString ManagerDataPaths::accountConfigPath(const QString &accountName) const
{
    const QString accountPath = path(ManagerDataPaths::PathType::Config, accountName);
    return accountPath;
}

QString ManagerDataPaths::accountConfigFileName(const QString &accountName) const
{
    const QString accountPath = accountConfigPath(accountName) + u"/ruqola.conf"_s;
    return accountPath;
}

QString ManagerDataPaths::path(ManagerDataPaths::PathType type, const QString &accountName, const QString &subdirectory) const
{
    QString path = mPathTypeHash.value(type);
    Q_ASSERT(!path.isEmpty());
    if (!accountName.isEmpty()) {
        path += u'/' + accountName;
    }
    switch (type) {
    case PathType::Picture:
    case PathType::Video:
        path += u"/Ruqola/recordings"_s;
        break;
    case PathType::PreviewUrl:
        path += u"/PreviewUrl"_s;
        break;
    case PathType::CustomSound:
        path += u"/CustomSound"_s;
        break;
    case PathType::Cache:
        path += u"/MainCache"_s;
        break;
    case PathType::Config:
        break;
    }
    if (!subdirectory.isEmpty()) {
        path += u'/' + subdirectory;
    }
    return path;
}

void ManagerDataPaths::initializePaths()
{
    mPathTypeHash.insert(PathType::Config, QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    mPathTypeHash.insert(PathType::Cache, QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    mPathTypeHash.insert(PathType::Video, QStandardPaths::writableLocation(QStandardPaths::MoviesLocation));
    mPathTypeHash.insert(PathType::Picture, QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    mPathTypeHash.insert(PathType::PreviewUrl, QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    mPathTypeHash.insert(PathType::CustomSound, QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    qCDebug(RUQOLA_LOG) << "mPathTypeHash:" << mPathTypeHash;
}
