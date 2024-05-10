/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "managerdatapaths.h"
#include "utils.h"
#include <QHash>
#include <QObject>
#include <QSet>

class Connection;
class RocketChatAccount;
class AvatarManager;
class LIBRUQOLACORE_TESTS_EXPORT RocketChatCache : public QObject
{
    Q_OBJECT
public:
    explicit RocketChatCache(RocketChatAccount *account, QObject *parent = nullptr);
    ~RocketChatCache() override;

    void setRestApiConnection(Connection *restApi);

    [[nodiscard]] QString recordingVideoPath(const QString &accountName) const;
    [[nodiscard]] QString recordingImagePath(const QString &accountName) const;
    [[nodiscard]] QString avatarUrl(const Utils::AvatarInfo &info);
    void insertAvatarUrl(const QString &userId, const QUrl &url);

    void downloadFileFromServer(const QString &filename, bool needAuthentication, ManagerDataPaths::PathType type = ManagerDataPaths::Cache);

    [[nodiscard]] QUrl attachmentUrlFromLocalCache(const QString &url);
    void downloadFile(const QString &url, const QUrl &localFile);
    [[nodiscard]] QString avatarUrlFromCacheOnly(const QString &userId);

    void updateAvatar(const Utils::AvatarInfo &info);
    [[nodiscard]] bool attachmentIsInLocalCache(const QString &url);
    [[nodiscard]] QUrl faviconLogoUrlFromLocalCache(const QString &url);
    [[nodiscard]] QUrl previewUrlFromLocalCache(const QString &url);
    [[nodiscard]] QUrl avatarUrlFromLocalCache(const QString &url);
    [[nodiscard]] QUrl soundUrlFromLocalCache(const QString &url);

Q_SIGNALS:
    void fileDownloaded(const QString &filePath, const QUrl &cacheImageUrl);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QUrl urlFromLocalCache(const QString &url,
                                                                 bool needAuthentication,
                                                                 ManagerDataPaths::PathType type = ManagerDataPaths::Cache);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT bool fileInCache(const QUrl &url);
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString fileCachePath(const QUrl &url, ManagerDataPaths::PathType type = ManagerDataPaths::Cache);
    LIBRUQOLACORE_NO_EXPORT void downloadAvatarFromServer(const Utils::AvatarInfo &info);
    LIBRUQOLACORE_NO_EXPORT void slotDataDownloaded(const QUrl &url, const QUrl &localFileUrl);
    LIBRUQOLACORE_NO_EXPORT void removeAvatar(const QString &avatarIdentifier);
    LIBRUQOLACORE_NO_EXPORT void loadAvatarCache();
    QHash<QString, QUrl> mAvatarUrl;
    QSet<QString> mFileInDownload;
    RocketChatAccount *const mAccount;
    AvatarManager *const mAvatarManager;
};
