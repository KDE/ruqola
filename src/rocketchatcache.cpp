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

#include "rocketchataccount.h"
#include "rocketchatcache.h"
#include "ruqola_debug.h"
#include "restapi/restapirequest.h"
#include <QDateTime>
#include <QDir>
#include <QSettings>
#include <QStandardPaths>

RocketChatCache::RocketChatCache(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mAccount(account)
{
    loadAvatarCache();
    connect(mAccount->restApi(), &RestApiRequest::avatar, this, &RocketChatCache::insertAvatarUrl);
    connect(mAccount->restApi(), &RestApiRequest::getDataDone, this, &RocketChatCache::slotDataDownloaded);
}

RocketChatCache::~RocketChatCache()
{
    QSettings settings;
    settings.beginGroup(QStringLiteral("Avatar"));
    QHash<QString, QString>::const_iterator i = mUserAvatarUrl.constBegin();
    while (i != mUserAvatarUrl.constEnd()) {
        settings.setValue(i.key(), i.value());
        ++i;
    }
    settings.endGroup();
}

void RocketChatCache::slotDataDownloaded(const QByteArray &data, const QUrl &url)
{
    //TODO

    QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    //Split between image/video/audio
    //TODO add accountName too.
    QDir().mkpath(cachePath);


    const QString newPath = cachePath + url.path();
    QFile file(newPath); //TODO
    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
    } else {
        qCWarning(RUQOLA_LOG) << file.errorString();
    }
    Q_EMIT fileDownloaded(url.toString(), newPath);
}

void RocketChatCache::loadAvatarCache()
{
    QSettings settings;
    settings.beginGroup(QStringLiteral("Avatar"));
    const QStringList keys = settings.childKeys();
    for (const QString &key : keys) {
        mUserAvatarUrl[key] = settings.value(key).toString();
    }
    settings.endGroup();
}

QString RocketChatCache::avatarUrl(const QString &userId)
{
    //avoid to call this method several time.
    if (!mUserAvatarUrl.contains(userId)) {
        mAccount->restApi()->getAvatar(userId);
        insertAvatarUrl(userId, QString());
        return {};
    } else {
        return mUserAvatarUrl.value(userId);
    }
}

void RocketChatCache::insertAvatarUrl(const QString &userId, const QString &url)
{
    mUserAvatarUrl.insert(userId, url);
    //TODO download avatar on local file.
    if (!url.isEmpty()) {
        mAccount->restApi()->get(QUrl(url + QLatin1String(".svg")));
    }
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
