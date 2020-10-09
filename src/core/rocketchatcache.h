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

#ifndef ROCKETCHATCACHE_H
#define ROCKETCHATCACHE_H

#include <QHash>
#include <QObject>
#include <QSet>
#include "libruqola_private_export.h"
class RocketChatAccount;
class AvatarManager;
class LIBRUQOLACORE_TESTS_EXPORT RocketChatCache : public QObject
{
    Q_OBJECT
public:
    explicit RocketChatCache(RocketChatAccount *account, QObject *parent = nullptr);
    ~RocketChatCache() override;

    Q_REQUIRED_RESULT QString recordingVideoPath(const QString &accountName) const;
    Q_REQUIRED_RESULT QString recordingImagePath(const QString &accountName) const;
    Q_REQUIRED_RESULT QString avatarUrl(const QString &avatarIdentifier);
    void insertAvatarUrl(const QString &userId, const QUrl &url);

    void downloadFileFromServer(const QString &filename);

    Q_REQUIRED_RESULT QUrl attachmentUrl(const QString &url);
    void downloadFile(const QString &url, const QUrl &localFile, bool storeInCache);
    Q_REQUIRED_RESULT QString avatarUrlFromCacheOnly(const QString &userId);
    Q_REQUIRED_RESULT QUrl urlForLink(const QString &link) const;

    void updateAvatar(const QString &avatarIdentifier);
Q_SIGNALS:
    void fileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);

private:
    Q_DISABLE_COPY(RocketChatCache)
    Q_REQUIRED_RESULT bool fileInCache(const QUrl &url);
    Q_REQUIRED_RESULT QString fileCachePath(const QUrl &url);
    void downloadAvatarFromServer(const QString &avatarIdentifier);
    void slotDataDownloaded(const QByteArray &data, const QUrl &url, bool storeInCache, const QUrl &localFileUrl);
    void removeAvatar(const QString &avatarIdentifier);
    void loadAvatarCache();
    QHash<QString, QUrl> mAvatarUrl;
    QSet<QString> mFileInDownload;
    RocketChatAccount *const mAccount;
    AvatarManager *mAvatarManager = nullptr;
};

#endif // ROCKETCHATCACHE_H
