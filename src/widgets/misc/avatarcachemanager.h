/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "pixmapcache.h"
#include "utils.h"
#include <QFont>
#include <QObject>

class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AvatarCacheManager : public QObject
{
    Q_OBJECT
public:
    explicit AvatarCacheManager(const Utils::AvatarType avatarType, QObject *parent = nullptr);
    ~AvatarCacheManager() override;

    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);

    [[nodiscard]] QPixmap makeAvatarPixmap(const QWidget *widget, const Utils::AvatarInfo &info, int maxHeight) const;
    [[nodiscard]] QPixmap makeAvatarEmojiPixmap(const QString &emojiStr, const QWidget *widget, const Utils::AvatarInfo &info, int maxHeight) const;
    [[nodiscard]] QPixmap makeAvatarUrlPixmap(const QWidget *widget, const QString &url, int maxHeight) const;

    void clearCache();

private:
    void slotAvatarChanged(const Utils::AvatarInfo &info);
    [[nodiscard]] qreal checkIfNeededToClearCache(const QWidget *widget) const;

    // DPR-dependent cache of avatars
    struct AvatarCache {
        qreal dpr = 0.;
        // For Avatar not necessary to limit cache. (cache)
        PixmapCache cache;
    };
    mutable AvatarCache mAvatarCache;
    const Utils::AvatarType mAvatarType;
    const QFont mEmojiFont;
    RocketChatAccount *mRocketChatAccount = nullptr;
};
