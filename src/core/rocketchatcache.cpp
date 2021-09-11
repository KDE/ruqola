/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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
#include "avatarmanager.h"
#include "connection.h"
#include "managerdatapaths.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
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
}

RocketChatCache::~RocketChatCache()
{
    QSettings settings(ManagerDataPaths::self()->accountAvatarConfigPath(mAccount->accountName()), QSettings::IniFormat);

    settings.beginGroup(QStringLiteral("Avatar"));
    QHash<QString, QUrl>::const_iterator i = mAvatarUrl.constBegin();
    while (i != mAvatarUrl.constEnd()) {
        if (!i.value().toString().isEmpty()) {
            settings.setValue(i.key(), i.value().toString());
        }
        ++i;
    }
    settings.endGroup();
}

void RocketChatCache::setRestApiConnection(RocketChatRestApi::Connection *restApi)
{
    connect(restApi, &RocketChatRestApi::Connection::downloadFileDone, this, &RocketChatCache::slotDataDownloaded);
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

void RocketChatCache::slotDataDownloaded(const QUrl &url, const QUrl &localFileUrl)
{
    mFileInDownload.remove(url.path());
    Q_EMIT fileDownloaded(url.path(), localFileUrl);
}

void RocketChatCache::loadAvatarCache()
{
    QSettings settings(ManagerDataPaths::self()->accountAvatarConfigPath(mAccount->accountName()), QSettings::IniFormat);
    settings.beginGroup(QStringLiteral("Avatar"));
    const QStringList keys = settings.childKeys();
    for (const QString &key : keys) {
        mAvatarUrl[key] = QUrl(settings.value(key).toString());
    }
    settings.endGroup();
}

void RocketChatCache::downloadFile(const QString &url, const QUrl &localFile)
{
    QFile f(fileCachePath(QUrl(url)));
    if (f.exists()) {
        if (!f.copy(localFile.toLocalFile())) {
            qCWarning(RUQOLA_LOG) << "Impossible to copy" << f.fileName() << "to" << localFile;
        }
        // emit fileDownloaded?
    } else {
        // Not in cache. We need to download it (e.g. file attachment).
        const QUrl downloadUrl = mAccount->urlForLink(url);
        // const QUrl destUrl = storeInCache ? QUrl::fromLocalFile(fileCachePath(downloadUrl)) : localFile;
        mAccount->restApi()->downloadFile(downloadUrl, localFile, QStringLiteral("text/plain"));
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

void RocketChatCache::downloadAvatarFromServer(const Utils::AvatarInfo &info)
{
    mAvatarManager->insertInDownloadQueue(info);
}

void RocketChatCache::downloadFileFromServer(const QString &filename)
{
    if (!mFileInDownload.contains(filename)) {
        mFileInDownload.insert(filename);
        const QUrl downloadUrl = mAccount->urlForLink(filename);
        const QUrl destFileUrl = QUrl::fromLocalFile(fileCachePath(downloadUrl));
        mAccount->restApi()->downloadFile(mAccount->urlForLink(filename), destFileUrl);
        // this will call slotDataDownloaded
    }
}

QString RocketChatCache::avatarUrlFromCacheOnly(const QString &userId)
{
    const QUrl avatarUrl = mAvatarUrl.value(userId);
    if (!avatarUrl.isEmpty() && fileInCache(avatarUrl)) {
        const QString url = QUrl::fromLocalFile(fileCachePath(avatarUrl)).toString();
        qCDebug(RUQOLA_LOG) << " Use image in cache" << url << " userId " << userId << " mUserAvatarUrl.value(userId) " << mAvatarUrl.value(userId);
        return url;
    }
    return {};
}

void RocketChatCache::removeAvatar(const QString &avatarIdentifier)
{
    const QUrl avatarUrl = mAvatarUrl.value(avatarIdentifier);
    if (avatarUrl.isEmpty()) {
        return;
    }
    QFile f(fileCachePath(avatarUrl));
    if (f.exists()) {
        if (!f.remove()) {
            qCWarning(RUQOLA_LOG) << "Impossible to remove f" << f.fileName() << " avartarUrl " << avatarUrl << " userIdentifier  " << avatarIdentifier;
        }
    }
}

void RocketChatCache::updateAvatar(const Utils::AvatarInfo &info)
{
    const QString avatarIdentifier = info.identifier;
    // qDebug() << " updateAvatar" << info;
    removeAvatar(avatarIdentifier);
    mAvatarUrl.remove(avatarIdentifier);
    insertAvatarUrl(avatarIdentifier, QUrl());
    downloadAvatarFromServer(info);
}

QString RocketChatCache::avatarUrl(const Utils::AvatarInfo &info)
{
    if (!info.isValid()) {
        return {};
    }
    const QString avatarIdentifier = info.identifier;
    // avoid to call this method several time.
    if (!mAvatarUrl.contains(avatarIdentifier)) {
        insertAvatarUrl(avatarIdentifier, QUrl());
        downloadAvatarFromServer(info);
        return {};
    } else {
        const QUrl valueUrl = mAvatarUrl.value(avatarIdentifier);
#if 0
        const QUrlQuery query{valueUrl};
        const QString etagValue = query.queryItemValue(QStringLiteral("etag"));
        if (info.etag.isEmpty() && !etagValue.isEmpty()) {
            insertAvatarUrl(avatarIdentifier, QUrl());
            downloadAvatarFromServer(info);
            return {};
        } else if (!info.etag.isEmpty() && etagValue.isEmpty()) {
            insertAvatarUrl(avatarIdentifier, QUrl());
            qDebug() << " redownload " << info;
            downloadAvatarFromServer(info);
            return {};
        }
#endif

        if (!valueUrl.isEmpty() && fileInCache(valueUrl)) {
            const QString url = QUrl::fromLocalFile(fileCachePath(valueUrl)).toString();
            // qDebug() << " Use image in cache" << url << " userId " << userId << " mUserAvatarUrl.value(userId) "<< mUserAvatarUrl.value(userId);

            return url;
        } else {
            downloadAvatarFromServer(info);
        }
        return {};
    }
}

void RocketChatCache::insertAvatarUrl(const QString &userIdentifier, const QUrl &url)
{
    mAvatarUrl.insert(userIdentifier, url);
    if (!url.isEmpty() && !fileInCache(url)) {
        mAccount->restApi()->downloadFile(url, QUrl::fromLocalFile(fileCachePath(url)), {});
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
