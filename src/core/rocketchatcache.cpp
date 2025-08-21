/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchatcache.h"
#include "avatarmanager.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
#include "ruqola_debug.h"
#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include <QSettings>
#include <QStandardPaths>
#include <QTimer>
#include <QUrlQuery>

using namespace std::chrono_literals;
using namespace Qt::Literals::StringLiterals;
RocketChatCache::RocketChatCache(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mAccount(account)
    , mAvatarManager(new AvatarManager(mAccount, this))
    , mAccountServerHost(Utils::generateServerUrl(account->serverUrl()).host())
{
    connect(mAvatarManager, &AvatarManager::insertAvatarUrl, this, &RocketChatCache::insertAvatarUrl);
    loadAvatarCache();

    auto cleanupTimer = new QTimer(this);
    cleanupTimer->setInterval(1h);
    connect(cleanupTimer, &QTimer::timeout, this, &RocketChatCache::cleanupCache);

    handleMigration();
}

RocketChatCache::~RocketChatCache()
{
    QSettings settings(ManagerDataPaths::self()->accountAvatarConfigPath(mAccount->accountName()), QSettings::IniFormat);

    settings.beginGroup(u"Avatar"_s);
    QHash<QString, QUrl>::const_iterator i = mAvatarUrl.constBegin();
    while (i != mAvatarUrl.constEnd()) {
        if (!i.value().toString().isEmpty()) {
            settings.setValue(i.key(), i.value().toString());
        }
        ++i;
    }
    settings.endGroup();
}

void RocketChatCache::setRestApiConnection(Connection *restApi)
{
    connect(restApi, &Connection::downloadFileDone, this, &RocketChatCache::slotDataDownloaded);
}

bool RocketChatCache::fileInCache(const QUrl &url)
{
    const QFileInfo f(fileCachePath(url));
    return f.exists();
}

QString RocketChatCache::fileCachePath(const QUrl &url, ManagerDataPaths::PathType type)
{
    QString cachePath = ManagerDataPaths::self()->path(type, mAccount->accountName());
    QString relativePathInCache = url.path();
    const QString host = url.host();
    if (!host.isEmpty() && host != mAccountServerHost) {
        relativePathInCache.prepend(host + u'/');
    }
    if (type == ManagerDataPaths::PathType::PreviewUrl) {
        relativePathInCache = relativePathInCache.replace(u'/', u'_');
    }
    cachePath += u'/' + relativePathInCache;
    if (url.hasQuery()) {
        const QUrlQuery query(url);
        cachePath += query.queryItemValue(u"etag"_s);
    }
    return cachePath;
}

void RocketChatCache::slotDataDownloaded(const QUrl &url, const QUrl &localFileUrl)
{
    mFileInDownload.remove(url);
    // TODO emit the complete QUrl rather than just the path
    Q_EMIT fileDownloaded(url.path(), localFileUrl);
}

void RocketChatCache::loadAvatarCache()
{
    QSettings settings(ManagerDataPaths::self()->accountAvatarConfigPath(mAccount->accountName()), QSettings::IniFormat);
    settings.beginGroup(u"Avatar"_s);
    const QStringList keys = settings.childKeys();
    for (const QString &key : keys) {
        mAvatarUrl[key] = QUrl(settings.value(key).toString());
    }
    settings.endGroup();
}

void RocketChatCache::cleanupCache()
{
    const QString cachePath = ManagerDataPaths::self()->path(ManagerDataPaths::Cache, mAccount->accountName());
    cleanupCacheDirectory(cachePath + "/file-upload"_L1);
    cleanupCacheDirectory(cachePath + "/media"_L1);
}

void RocketChatCache::cleanupCacheDirectory(const QString &directory)
{
    qCDebug(RUQOLA_LOG) << "Checking cache directory" << directory;
    const QDate today = QDate::currentDate();
    QDirIterator it(directory, {}, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        const QFileInfo fileInfo(it.next());
        if (fileInfo.lastModified().date().daysTo(today) > 365) { // at least one year old
            qCInfo(RUQOLA_LOG) << "Deleting" << fileInfo.absoluteFilePath();
            QFile::remove(fileInfo.absoluteFilePath());
        }
    }
    // Clean up empty directories
    QDirIterator dirit(directory, {}, QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (dirit.hasNext()) {
        const QString path = dirit.next();
        const QDir dir(path);
        if (dir.isEmpty()) {
            qCInfo(RUQOLA_LOG) << "Deleting empty dir" << path;
            QDir().rmdir(path);
        }
    }
}

void RocketChatCache::handleMigration()
{
    const int version = mAccount->settings()->cacheVersion();
    if (version == 0) {
        removeCache();
        mAccount->settings()->setCacheVersion(1);
    }
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
        mAccount->restApi()->downloadFile(downloadUrl, localFile, QByteArray("text/plain"));
        // this will call slotDataDownloaded
    }
}

bool RocketChatCache::attachmentIsInLocalCache(const QString &url)
{
    const QString cachePath = fileCachePath(QUrl(url));
    if (QFileInfo::exists(cachePath)) {
        return true;
    }
    return false;
}

QUrl RocketChatCache::faviconLogoUrlFromLocalCache(const QString &url)
{
    return urlFromLocalCache(url, false);
}

QUrl RocketChatCache::avatarUrlFromLocalCache(const QString &url)
{
    return urlFromLocalCache(url, false);
}

QUrl RocketChatCache::urlFromLocalCache(const QString &url, bool needAuthentication, ManagerDataPaths::PathType type)
{
    if (url.isEmpty()) {
        return {};
    }
    const QString cachePath = fileCachePath(QUrl(url), type);
    if (QFileInfo::exists(cachePath)) {
        // QML wants a QUrl here. The widgets code would be simpler with just a QString path.
        return QUrl::fromLocalFile(cachePath);
    } else {
        downloadFileFromServer(url, needAuthentication, type);
    }
    return {};
}

QUrl RocketChatCache::soundUrlFromLocalCache(const QString &url)
{
    const QUrl soundUrl = urlFromLocalCache(url, false, ManagerDataPaths::CustomSound);
    // qDebug() << "soundUrlFromLocalCache  " << previewUrl;
    return soundUrl;
}

void RocketChatCache::removeCache()
{
    // Don't clear all cache when we create new account
    if (mAccount->accountName().isEmpty()) {
        return;
    }
    const QString storeCachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + u'/' + mAccount->accountName() + u'/';
    QDir dir(storeCachePath);
    if (dir.exists()) {
        qDebug() << "Deleting old cache dir" << storeCachePath;
        if (!dir.removeRecursively()) {
            qCWarning(RUQOLA_LOG) << "Impossible to delete cache dir:" << storeCachePath;
        }
    }
}

QUrl RocketChatCache::previewUrlFromLocalCache(const QString &url)
{
    const QUrl previewUrl = urlFromLocalCache(url, false, ManagerDataPaths::PreviewUrl);
    // qDebug() << "previewUrl  " << previewUrl;
    return previewUrl;
}

QUrl RocketChatCache::attachmentUrlFromLocalCache(const QString &url)
{
    return urlFromLocalCache(url, true);
}

void RocketChatCache::downloadAvatarFromServer(const Utils::AvatarInfo &info)
{
    mAvatarManager->insertInDownloadQueue(info);
}

void RocketChatCache::downloadFileFromServer(const QString &filename, bool needAuthentication, ManagerDataPaths::PathType type)
{
    const QUrl downloadUrl = mAccount->urlForLink(filename);
    if (!mFileInDownload.contains(downloadUrl)) {
        mFileInDownload.insert(downloadUrl);
        const QUrl destFileUrl = QUrl::fromLocalFile(fileCachePath(downloadUrl, type));
        mAccount->restApi()->downloadFile(downloadUrl, destFileUrl, "text/plain", needAuthentication);
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
    const QString avatarIdentifier = info.generateAvatarIdentifier();
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
    // const QString avatarIdentifier = info.identifier;
    const QString avatarIdentifier = info.generateAvatarIdentifier();
    // qDebug() << " RocketChatCache::avatarUrl " << avatarIdentifier;
    // avoid to call this method several time.
    if (!mAvatarUrl.contains(avatarIdentifier)) {
        insertAvatarUrl(avatarIdentifier, QUrl());
        downloadAvatarFromServer(info);
        return {};
    } else {
        const QUrl valueUrl = mAvatarUrl.value(avatarIdentifier);
        // qDebug() << " valueUrl " << valueUrl;
#if 0
        const QUrlQuery query{valueUrl};
        const QString etagValue = query.queryItemValue(u"etag"_s);
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
            // qDebug() << "Use image in cache  " << url;

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
        mAccount->restApi()->downloadFile(url, QUrl::fromLocalFile(fileCachePath(url)), "image/png"_ba);
        // this will call slotDataDownloaded
    }
}

QString RocketChatCache::recordingVideoPath(const QString &accountName) const
{
    const QString path = ManagerDataPaths::self()->path(ManagerDataPaths::Video, accountName);
    if (!QDir().mkpath(path)) {
        qCWarning(RUQOLA_LOG) << "Unable to create video folder: " << path;
        return {};
    }
    const QString filePath = path + u'/' + QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()) + ".mp4"_L1;
    return filePath;
}

QString RocketChatCache::recordingImagePath(const QString &accountName) const
{
    const QString path = ManagerDataPaths::self()->path(ManagerDataPaths::Picture, accountName);
    if (!QDir().mkpath(path)) {
        qCWarning(RUQOLA_LOG) << "Unable to create picture folder: " << path;
        return {};
    }
    const QString filePath = path + u'/' + QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()) + ".jpg"_L1;
    return filePath;
}

#include "moc_rocketchatcache.cpp"
