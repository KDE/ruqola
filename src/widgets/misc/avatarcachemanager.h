/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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


#ifndef AVATARCACHEMANAGER_H
#define AVATARCACHEMANAGER_H

#include <QObject>
#include "utils.h"
#include "libruqolawidgets_private_export.h"
#include "pixmapcache.h"
#include <QFont>

class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AvatarCacheManager : public QObject
{
    Q_OBJECT
public:
    explicit AvatarCacheManager(const Utils::AvatarType avatarType, QObject *parent = nullptr);
    ~AvatarCacheManager();

    void setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount);

    Q_REQUIRED_RESULT QPixmap makeAvatarUrlPixmap(const QWidget *widget, const QString &identifier, int maxHeight) const;
    Q_REQUIRED_RESULT QPixmap makeAvatarEmojiPixmap(const QString &emojiStr, const QWidget *widget, const QString &identifier, int maxHeight) const;
public Q_SLOTS:
    void slotAvatarChanged(const Utils::AvatarInfo &info);

private:

    Q_REQUIRED_RESULT qreal checkIfNeededToClearCache(const QWidget *widget) const;

    // DPR-dependent cache of avatars
    struct AvatarCache {
        qreal dpr = 0.;
        PixmapCache cache;
    };
    mutable AvatarCache mAvatarCache;
    const Utils::AvatarType mAvatarType;
    const QFont mEmojiFont;
    RocketChatAccount *mRocketChatAccount = nullptr;
};

#endif // AVATARCACHEMANAGER_H
