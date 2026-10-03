/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "avatarcachemanager.h"
#include "emoticons/emojimanager.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <QPainter>
#include <QPainterPath>
#include <QWidget>

AvatarCacheManager::AvatarCacheManager(const Utils::AvatarType avatarType, QObject *parent)
    : QObject(parent)
    , mAvatarType(avatarType)
    , mEmojiFont(Utils::emojiFontName())
{
}

AvatarCacheManager::~AvatarCacheManager() = default;

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
    if ((info.avatarType == mAvatarType) || (mAvatarType == Utils::AvatarType::UserAndRoom)) {
        const QString iconUrlStr = mRocketChatAccount->avatarUrl(info);
        if (iconUrlStr.isEmpty()) {
            return;
        }
        auto &cache = mAvatarCache.cache;
        auto downScaled = cache.findCachedPixmap(iconUrlStr);
        if (!downScaled.isNull()) {
            mAvatarCache.cache.remove(iconUrlStr);
            mRocketChatAccount->updateAvatarCache(info);
        }
    }
}

void AvatarCacheManager::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    if (mRocketChatAccount) {
        disconnect(mRocketChatAccount, nullptr, this, nullptr);
    }

    mRocketChatAccount = currentRocketChatAccount;
    if (mRocketChatAccount) {
        connect(mRocketChatAccount, &RocketChatAccount::avatarWasChanged, this, &AvatarCacheManager::slotAvatarChanged);
    }
}

QPixmap AvatarCacheManager::makeAvatarEmojiPixmap(const QString &emojiStr, const QWidget *widget, const Utils::AvatarInfo &info, int maxHeight) const
{
    const auto dpr = checkIfNeededToClearCache(widget);
    auto &cache = mAvatarCache.cache;

    auto downScaled = cache.findCachedPixmap(emojiStr);
    if (downScaled.isNull()) {
        auto *emojiManager = mRocketChatAccount->emojiManager();
        const TextEmoticonsCore::UnicodeEmoticon emoticon = emojiManager->unicodeEmoticonForEmoji(emojiStr);
        if (!emoticon.isValid()) {
            return makeAvatarPixmap(widget, info, maxHeight);
        }
        const int targetHeight = qMax(1, qRound(maxHeight * dpr));
        // Draw the glyph at (roughly) its final device size: rendering at the font's natural size and
        // upscaling afterwards made emoji avatars blurry on hidpi screens.
        QFont emojiFont(mEmojiFont);
        emojiFont.setPixelSize(targetHeight);
        const QFontMetrics fm(emojiFont);
        const QSize size = fm.boundingRect(emoticon.unicode()).size();

        // qDebug() << " size " << size << "emojiStr "<< emojiStr << " emoticon.unicode() " <<emoticon.unicode() <<
        // fm.horizontalAdvance(emoticon.unicode()); boundingRect can return a width == 0 for existing character as :warning: emoji.
        QPixmap fullScale(fm.horizontalAdvance(emoticon.unicode()), size.height());

        // The avatar is drawn over the view background, so the emoji must not carry an opaque
        // rectangle of its own.
        fullScale.fill(Qt::transparent);
        QPainter painter(&fullScale);
        painter.setFont(emojiFont);
        // Emojis without a color glyph fall back to the pen color, which defaults to black: follow the
        // palette so that they stay readable with a dark color scheme.
        painter.setPen(widget->palette().color(QPalette::WindowText));
        painter.drawText(fullScale.rect(), Qt::AlignCenter, emoticon.unicode());
        painter.end();
        downScaled = fullScale.scaledToHeight(targetHeight, Qt::SmoothTransformation);
        downScaled.setDevicePixelRatio(dpr);
        cache.insertCachedPixmap(emojiStr, downScaled);
    }
    return downScaled;
}

void AvatarCacheManager::clearCache()
{
    mAvatarCache.cache.clear();
}

void AvatarCacheManager::setMaxEntries(int maxEntries)
{
    mAvatarCache.cache.setMaxEntries(maxEntries);
}

QPixmap AvatarCacheManager::makeAvatarUrlPixmap(const QWidget *widget, const QString &url, int maxHeight) const
{
    const QUrl iconUrl = mRocketChatAccount->previewUrlFromLocalCache(url);
    if (iconUrl.isEmpty()) {
        return {};
    }

    const auto dpr = checkIfNeededToClearCache(widget);

    auto &cache = mAvatarCache.cache;

    const QString iconUrlLocalFile = iconUrl.toLocalFile();
    auto downScaled = cache.findCachedPixmap(iconUrlLocalFile);
    if (downScaled.isNull()) {
        Q_ASSERT(iconUrl.isLocalFile());
        QPixmap fullScale;
        if (!fullScale.load(iconUrlLocalFile)) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Could not load" << iconUrlLocalFile;
            return {};
        }
        downScaled = fullScale.scaledToHeight(maxHeight * dpr, Qt::SmoothTransformation);
        downScaled.setDevicePixelRatio(dpr);
        cache.insertCachedPixmap(iconUrlLocalFile, downScaled);
    }
    return downScaled;
}

QPixmap AvatarCacheManager::makeAvatarPixmap(const QWidget *widget, const Utils::AvatarInfo &info, int maxHeight) const
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

QPixmap AvatarCacheManager::makeRoundedAvatarPixmap(const QWidget *widget, const Utils::AvatarInfo &info, int maxHeight) const
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
        QPixmap pix;
        if (!pix.load(iconUrl.toLocalFile())) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Could not load" << iconUrl.toLocalFile();
            return {};
        }
        pix = pix.scaledToHeight(maxHeight * dpr, Qt::SmoothTransformation);
        pix.setDevicePixelRatio(dpr);

        downScaled = QPixmap(pix.size());
        downScaled.fill(Qt::transparent);
        downScaled.setDevicePixelRatio(dpr);

        QPainterPath path;
        QPainter p(&downScaled);
        p.setRenderHint(QPainter::Antialiasing);
        // The painter paints in device-independent pixels, so the clip path must be expressed in
        // them too: QPixmap::rect() is in device pixels and would be dpr times too large.
        path.addRoundedRect(QRectF(QPointF(0, 0), downScaled.deviceIndependentSize()), 5, 5);

        p.setClipPath(path);
        p.drawPixmap(QPoint(0, 0), pix);
        p.end();
        cache.insertCachedPixmap(iconUrlStr, downScaled);
    }
    return downScaled;
}
#include "moc_avatarcachemanager.cpp"
