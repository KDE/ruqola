/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "utils.h"
#include <QHash>
#include <QObject>
#include <QSet>

namespace RocketChatRestApi
{
class Connection;
}

class RocketChatAccount;
class AvatarManager;
class LIBRUQOLACORE_TESTS_EXPORT RocketChatCache : public QObject
{
    Q_OBJECT
public:
    explicit RocketChatCache(RocketChatAccount *account, QObject *parent = nullptr);
    ~RocketChatCache() override;

    void setRestApiConnection(RocketChatRestApi::Connection *restApi);

    Q_REQUIRED_RESULT QString recordingVideoPath(const QString &accountName) const;
    Q_REQUIRED_RESULT QString recordingImagePath(const QString &accountName) const;
    Q_REQUIRED_RESULT QString avatarUrl(const Utils::AvatarInfo &info);
    void insertAvatarUrl(const QString &userId, const QUrl &url);

    void downloadFileFromServer(const QString &filename);

    Q_REQUIRED_RESULT QUrl attachmentUrlFromLocalCache(const QString &url);
    void downloadFile(const QString &url, const QUrl &localFile);
    Q_REQUIRED_RESULT QString avatarUrlFromCacheOnly(const QString &userId);

    void updateAvatar(const Utils::AvatarInfo &info);
    Q_REQUIRED_RESULT bool attachmentIsInLocalCache(const QString &url);
Q_SIGNALS:
    void fileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);

private:
    Q_DISABLE_COPY(RocketChatCache)
    Q_REQUIRED_RESULT bool fileInCache(const QUrl &url);
    Q_REQUIRED_RESULT QString fileCachePath(const QUrl &url);
    void downloadAvatarFromServer(const Utils::AvatarInfo &info);
    void slotDataDownloaded(const QUrl &url, const QUrl &localFileUrl);
    void removeAvatar(const QString &avatarIdentifier);
    void loadAvatarCache();
    QHash<QString, QUrl> mAvatarUrl;
    QSet<QString> mFileInDownload;
    RocketChatAccount *const mAccount;
    AvatarManager *mAvatarManager = nullptr;
};
