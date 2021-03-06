/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "avatarcachemanager.h"
#include "emoticons/emojimanager.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <QPainter>
#include <QWidget>

AvatarCacheManager::AvatarCacheManager(const Utils::AvatarType avatarType, QObject *parent)
    : QObject(parent)
    , mAvatarType(avatarType)
    , mEmojiFont(Utils::emojiFontName())
{
}

AvatarCacheManager::~AvatarCacheManager()
{
}

qreal AvatarCacheManager::checkIfNeededToClearCache(const QWidget *widget) const
{
    const auto dpr = widget->devicePixelRatioF();
    if (dpr != mAvatarCache.dpr) {
        mAvatarCache.dpr = dpr;
        mAvatarCache.cache.clear();
    }
    return dpr;
}

void AvatarCacheManager::slotAvatarChanged(const Utils::AvatarInfo &info)
{
    if (info.avatarType == mAvatarType) {
        const QString iconUrlStr = mRocketChatAccount->avatarUrl(info);
        if (iconUrlStr.isEmpty()) {
            return;
        }
        auto &cache = mAvatarCache.cache;
        auto downScaled = cache.findCachedPixmap(iconUrlStr);
        // Perhaps we can optimize it and not cleaning all cache, only pixmap from useridentifier.
        if (!downScaled.isNull()) {
            mAvatarCache.cache.remove(iconUrlStr);
        }
    }
}

void AvatarCacheManager::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    if (mRocketChatAccount) {
        disconnect(mRocketChatAccount, nullptr, this, nullptr);
    }

    mRocketChatAccount = currentRocketChatAccount;
    connect(mRocketChatAccount, &RocketChatAccount::avatarWasChanged, this, &AvatarCacheManager::slotAvatarChanged);
}

QPixmap AvatarCacheManager::makeAvatarEmojiPixmap(const QString &emojiStr, const QWidget *widget, const Utils::AvatarInfo &info, int maxHeight) const
{
    const auto dpr = checkIfNeededToClearCache(widget);
    auto &cache = mAvatarCache.cache;

    auto downScaled = cache.findCachedPixmap(emojiStr);
    if (downScaled.isNull()) {
        auto *emojiManager = mRocketChatAccount->emojiManager();
        const UnicodeEmoticon emoticon = emojiManager->unicodeEmoticonForEmoji(emojiStr);
        if (emoticon.isValid()) {
            const QFontMetrics fm(mEmojiFont);
            const QSize size = fm.boundingRect(emoticon.unicode()).size();

            // qDebug() << " size " << size << "emojiStr "<< emojiStr << " emoticon.unicode() " <<emoticon.unicode() <<
            // fm.horizontalAdvance(emoticon.unicode()); boundingRect can return a width == 0 for existing character as :warning: emoji.
            QPixmap fullScale(fm.horizontalAdvance(emoticon.unicode()), size.height());

            fullScale.fill(Qt::white);
            QPainter painter(&fullScale);
            painter.setFont(mEmojiFont);
            painter.drawText(fullScale.rect(), Qt::AlignCenter, emoticon.unicode());
            downScaled = fullScale.scaledToHeight(maxHeight * dpr, Qt::SmoothTransformation);
            downScaled.setDevicePixelRatio(dpr);
            cache.insertCachedPixmap(emojiStr, downScaled);
        } else {
            return makeAvatarUrlPixmap(widget, info, maxHeight);
        }
    }
    return downScaled;
}

QPixmap AvatarCacheManager::makeAvatarUrlPixmap(const QWidget *widget, const Utils::AvatarInfo &info, int maxHeight) const
{
    const QString iconUrlStr = mRocketChatAccount->avatarUrl(info);
    if (iconUrlStr.isEmpty()) {
        return {};
    }

    const auto dpr = checkIfNeededToClearCache(widget);

    auto &cache = mAvatarCache.cache;

    auto downScaled = cache.findCachedPixmap(iconUrlStr);
    if (downScaled.isNull()) {
        const QUrl iconUrl(iconUrlStr);
        Q_ASSERT(iconUrl.isLocalFile());
        QPixmap fullScale;
        if (!fullScale.load(iconUrl.toLocalFile())) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Could not load" << iconUrl.toLocalFile();
            return {};
        }
        downScaled = fullScale.scaledToHeight(maxHeight * dpr, Qt::SmoothTransformation);
        downScaled.setDevicePixelRatio(dpr);
        cache.insertCachedPixmap(iconUrlStr, downScaled);
    }
    return downScaled;
}
