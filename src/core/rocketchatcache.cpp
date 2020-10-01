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

#include "rocketchataccount.h"
#include "rocketchatcache.h"
#include "ruqola_debug.h"
#include "managerdatapaths.h"
#include "restapirequest.h"
#include "avatarmanager.h"
#include <QDateTime>
#include <QDir>
#include <QSettings>

RocketChatCache::RocketChatCache(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mAccount(account)
{
    mAvatarManager = new AvatarManager(mAccount, this);
    connect(mAvatarManager, &AvatarManager::insertAvatarUrl, this, &RocketChatCache::insertAvatarUrl);
    loadAvatarCache();
    connect(mAccount->restApi(), &RocketChatRestApi::RestApiRequest::downloadFileDone, this, &RocketChatCache::slotDataDownloaded);
}

RocketChatCache::~RocketChatCache()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("Avatar"));
    QHash<QString, QUrl>::const_iterator i = mUserAvatarUrl.constBegin();
    while (i != mUserAvatarUrl.constEnd()) {
        if (!i.value().toString().isEmpty()) {
            settings.setValue(i.key(), i.value().toString());
        }
        ++i;
    }
    settings.endGroup();
}

bool RocketChatCache::fileInCache(const QUrl &url)
{
    const QFileInfo f(fileCachePath(url));
    return f.exists();
}

QString RocketChatCache::fileCachePath(const QUrl &url)
{
    QString cachePath = ManagerDataPaths::self()->path(ManagerDataPaths::Cache, mAccount->accountName());
    cachePath += QLatin1Char('/') + url.path();
    return cachePath;
}

void RocketChatCache::slotDataDownloaded(const QByteArray &data, const QUrl &url, bool storeInCache, const QUrl &localFileUrl)
{
    mFileInDownload.remove(url.path());
    const QString newPath = storeInCache ? fileCachePath(url) : localFileUrl.toLocalFile();
    //Split between image/video/audio
    const QUrl urldir = QUrl::fromUserInput(newPath).adjusted(QUrl::RemoveFilename);
    QDir().mkpath(urldir.toLocalFile());
    QFile file(newPath);
    if (file.open(QIODevice::ReadWrite)) {
        file.write(data);
        file.close();
        Q_EMIT fileDownloaded(url.path(), QUrl::fromLocalFile(newPath));
    } else {
        qCWarning(RUQOLA_LOG) <<" Error !" <<  file.errorString();
    }
}

void RocketChatCache::loadAvatarCache()
{
    QSettings settings;
    settings.beginGroup(QStringLiteral("Avatar"));
    const QStringList keys = settings.childKeys();
    for (const QString &key : keys) {
        mUserAvatarUrl[key] = QUrl(settings.value(key).toString());
    }
    settings.endGroup();
}

void RocketChatCache::downloadFile(const QString &url, const QUrl &localFile, bool storeInCache)
{
    QFile f(fileCachePath(QUrl(url)));
    if (f.exists()) {
        if (!f.copy(localFile.toLocalFile())) {
            qCWarning(RUQOLA_LOG) << "Impossible to copy" << f.fileName() << "to" << localFile;
        }
    } else {
        // Not in cache. We need to download it (e.g. file attachment).
        const QUrl downloadUrl = urlForLink(url);
        mAccount->restApi()->downloadFile(downloadUrl, QStringLiteral("text/plain"), storeInCache, localFile);
        // this will call slotDataDownloaded
    }
}

QUrl RocketChatCache::attachmentUrl(const QString &url)
{
    const QString cachePath = fileCachePath(QUrl(url));
    if (QFileInfo::exists(cachePath)) {
        // QML wants a QUrl here. The widgets code would be simpler with just a QString path.
        return QUrl::fromLocalFile(cachePath);
    } else {
        downloadFileFromServer(url);
    }
    return {};
}

void RocketChatCache::downloadAvatarFromServer(const QString &userId)
{
    if (!userId.isEmpty()) {
        mAvatarManager->insertInDownloadQueue(userId);
    }
}

void RocketChatCache::downloadFileFromServer(const QString &filename)
{
    if (!mFileInDownload.contains(filename)) {
        mFileInDownload.insert(filename);
        mAccount->restApi()->downloadFile(urlForLink(filename));
        // this will call slotDataDownloaded
    }
}

QUrl RocketChatCache::urlForLink(const QString &link) const
{
    if (link.startsWith(QLatin1String("https:")) || link.startsWith(QLatin1String("http:"))) {
        return QUrl(link);
    }
    QString tmpUrl = mAccount->settings()->serverUrl();
    if (!tmpUrl.startsWith(QLatin1String("https://"))) {
        tmpUrl = QLatin1String("https://") + tmpUrl;
    }
    const QUrl downloadFileUrl = QUrl::fromUserInput(tmpUrl + link);
    return downloadFileUrl;
}

QString RocketChatCache::avatarUrlFromCacheOnly(const QString &userId)
{
    const QUrl avatarUrl = mUserAvatarUrl.value(userId);
    if (!avatarUrl.isEmpty() && fileInCache(avatarUrl)) {
        const QString url = QUrl::fromLocalFile(fileCachePath(avatarUrl)).toString();
        qCDebug(RUQOLA_LOG) << " Use image in cache" << url << " userId " << userId << " mUserAvatarUrl.value(userId) "<< mUserAvatarUrl.value(userId);
        return url;
    }
    return {};
}

void RocketChatCache::removeAvatar(const QString &userIdentifier)
{
    const QUrl avatarUrl = mUserAvatarUrl.value(userIdentifier);
    QFile f(fileCachePath(avatarUrl));
    if (f.exists()) {
        if (!f.remove()) {
            qCWarning(RUQOLA_LOG) << "Impossible to remove f" << f.fileName();
        }
    }
}

void RocketChatCache::updateAvatar(const QString &userIdentifier)
{
    removeAvatar(userIdentifier);
    mUserAvatarUrl.remove(userIdentifier);
    insertAvatarUrl(userIdentifier, QUrl());
    downloadAvatarFromServer(userIdentifier);
}

QString RocketChatCache::avatarUrl(const QString &userId)
{
    //avoid to call this method several time.
    if (!mUserAvatarUrl.contains(userId)) {
        insertAvatarUrl(userId, QUrl());
        downloadAvatarFromServer(userId);
        return {};
    } else {
        const QUrl valueUrl = mUserAvatarUrl.value(userId);
        if (!valueUrl.isEmpty() && fileInCache(valueUrl)) {
            const QString url = QUrl::fromLocalFile(fileCachePath(valueUrl)).toString();
            //qDebug() << " Use image in cache" << url << " userId " << userId << " mUserAvatarUrl.value(userId) "<< mUserAvatarUrl.value(userId);

            return url;
        } else {
            downloadAvatarFromServer(userId);
        }
        return {};
    }
}

void RocketChatCache::insertAvatarUrl(const QString &userIdentifier, const QUrl &url)
{
    mUserAvatarUrl.insert(userIdentifier, url);
    if (!url.isEmpty() && !fileInCache(url)) {
        mAccount->restApi()->downloadFile(url);
        // this will call slotDataDownloaded
    }
}

QString RocketChatCache::recordingVideoPath(const QString &accountName) const
{
    const QString path = ManagerDataPaths::self()->path(ManagerDataPaths::Video, accountName);
    if (!QDir().mkpath(path)) {
        qCWarning(RUQOLA_LOG) << "Unable to create folder: " << path;
        return QString();
    }
    const QString filePath = path + QLatin1Char('/') + QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()) + QStringLiteral(".mp4");
    return filePath;
}

QString RocketChatCache::recordingImagePath(const QString &accountName) const
{
    const QString path = ManagerDataPaths::self()->path(ManagerDataPaths::Picture, accountName);
    if (!QDir().mkpath(path)) {
        qCWarning(RUQOLA_LOG) << "Unable to create folder: " << path;
        return QString();
    }
    const QString filePath = path + QLatin1Char('/') + QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()) + QStringLiteral(".jpg");
    return filePath;
}
