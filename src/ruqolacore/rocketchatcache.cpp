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
#include "managerdatapaths.h"
#include "restapirequest.h"
#include "copyfilejob.h"
#include <QDateTime>
#include <QDir>
#include <QSettings>

RocketChatCache::RocketChatCache(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mAccount(account)
{
    loadAvatarCache();
    connect(mAccount->restApi(), &RocketChatRestApi::RestApiRequest::avatar, this, &RocketChatCache::insertAvatarUrl);
    connect(mAccount->restApi(), &RocketChatRestApi::RestApiRequest::downloadFileDone, this, &RocketChatCache::slotDataDownloaded);
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
    const QString newPath = storeInCache ? fileCachePath(url) : localFileUrl.path();
    //Split between image/video/audio
    const QUrl urldir = QUrl::fromUserInput(newPath).adjusted(QUrl::RemoveFilename);
    QDir().mkpath(urldir.path());
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
        mUserAvatarUrl[key] = settings.value(key).toString();
    }
    settings.endGroup();
}

void RocketChatCache::downloadFile(const QString &url, const QUrl &localFile, bool storeInCache)
{
    if (fileInCache(QUrl(url))) {
        const QUrl newurl = QUrl::fromLocalFile(fileCachePath(QUrl(url)));
        CopyFileJob *job = new CopyFileJob(this);
        job->setCachedFile(fileCachePath(QUrl(url)));
        job->setLocalFile(localFile.toString());
        job->start();
    } else {
        //Redownload it. TODO inform user ?
        //FIXME we don't use localfile!
        const QUrl clickedUrl = generateDownloadFile(url);
        mAccount->restApi()->downloadFile(clickedUrl, QStringLiteral("text/plain"), storeInCache, localFile);
    }
}

QUrl RocketChatCache::attachmentUrl(const QString &url)
{
    if (fileInCache(QUrl(url))) {
        const QUrl newurl = QUrl::fromLocalFile(fileCachePath(QUrl(url)));
        return newurl;
    } else {
        downloadFileFromServer(url);
    }
    return {};
}

void RocketChatCache::downloadAvatarFromServer(const QString &userId)
{
    mAccount->restApi()->getAvatar(userId);
}

void RocketChatCache::downloadFileFromServer(const QString &filename)
{
    mAccount->restApi()->downloadFile(generateDownloadFile(filename));
}

QUrl RocketChatCache::generateDownloadFile(const QString &url)
{
    if (url.startsWith(QLatin1String("https:")) || url.startsWith(QLatin1String("http:"))) {
        return QUrl(url);
    }
    QString tmpUrl = mAccount->settings()->serverUrl();
    if (!tmpUrl.startsWith(QLatin1String("https://"))) {
        tmpUrl = QStringLiteral("https://") + tmpUrl;
    }
    const QUrl downloadFileUrl = QUrl::fromUserInput(tmpUrl + url);
    return downloadFileUrl;
}

QString RocketChatCache::avatarUrlFromCacheOnly(const QString &userId)
{
    const QString valueId = mUserAvatarUrl.value(userId);
    if (!valueId.isEmpty() && fileInCache(QUrl::fromUserInput(valueId))) {
        const QString url = QUrl::fromLocalFile(fileCachePath(QUrl::fromUserInput(valueId))).toString();
        qCDebug(RUQOLA_LOG) << " Use image in cache" << url << " userId " << userId << " mUserAvatarUrl.value(userId) "<< mUserAvatarUrl.value(userId);
        return url;
    }
    return {};
}

QString RocketChatCache::avatarUrl(const QString &userId)
{
    //avoid to call this method several time.
    if (!mUserAvatarUrl.contains(userId)) {
        insertAvatarUrl(userId, QString());
        downloadAvatarFromServer(userId);
        return {};
    } else {
        const QString valueId = mUserAvatarUrl.value(userId);
        if (!valueId.isEmpty() && fileInCache(QUrl::fromUserInput(valueId))) {
            const QString url = QUrl::fromLocalFile(fileCachePath(QUrl::fromUserInput(valueId))).toString();
            qCDebug(RUQOLA_LOG) << " Use image in cache" << url << " userId " << userId << " mUserAvatarUrl.value(userId) "<< mUserAvatarUrl.value(userId);
            return url;
        } else {
            downloadAvatarFromServer(userId);
        }
        return {};
    }
}

void RocketChatCache::insertAvatarUrl(const QString &userId, const QString &url)
{
    mUserAvatarUrl.insert(userId, url);
    if (!url.isEmpty()) {
        mAccount->restApi()->downloadFile(QUrl(url));
    }
}

QString RocketChatCache::recordingVideoPath(const QString &accountName) const
{
    const QString path = ManagerDataPaths::self()->path(ManagerDataPaths::Video, accountName);
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
    const QString path = ManagerDataPaths::self()->path(ManagerDataPaths::Picture, accountName);
    QDir directory(path);
    if (!directory.mkpath(path)) {
        qCWarning(RUQOLA_LOG) << "Unable to create folder: " << path;
        return QString();
    }
    const QString filePath = path + QLatin1Char('/') + QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()) + QStringLiteral(".jpg");
    return filePath;
}
