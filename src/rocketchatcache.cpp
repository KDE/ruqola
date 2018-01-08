/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "rocketchatcache.h"
#include "ruqola_debug.h"
#include <QDateTime>
#include <QDir>
#include <QStandardPaths>

RocketChatCache::RocketChatCache(QObject *parent)
    : QObject(parent)
{

}

RocketChatCache::~RocketChatCache()
{

}

QString RocketChatCache::recordingVideoPath(const QString &accountName) const
{
    const QString path = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + accountName + QStringLiteral("/recordings");
    QDir directory(path);
    if (!directory.mkpath(path)) {
        qCWarning(RUQOLA_LOG) << "Unable to create folder: " << path;
        return QString();
    }
    const QString filePath = path + QLatin1Char('/') + QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()) + QStringLiteral(".mp4");
    return filePath;
}

QString RocketChatCache::recordingImagePath(const QString &accountName) const
{
    const QString path = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + accountName + QStringLiteral("/recordings");
    QDir directory(path);
    if (!directory.mkpath(path)) {
        qCWarning(RUQOLA_LOG) << "Unable to create folder: " << path;
        return QString();
    }
    const QString filePath = path + QLatin1Char('/') + QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()) + QStringLiteral(".jpg");
    return filePath;
}
