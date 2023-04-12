/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojicompletiondelegate.h"
#include "common/delegatepaintutil.h"
#include "utils.h"
#include <model/emoticonmodel.h>

#include <QPainter>

EmojiCompletionDelegate::EmojiCompletionDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mEmojiFont(Utils::emojiFontName())
{
}

EmojiCompletionDelegate::~EmojiCompletionDelegate() = default;

void EmojiCompletionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] <unicode emoji> <:identifier:>     ([M] = margin)
    drawBackground(painter, option, index);
    const int margin = DelegatePaintUtil::margin();
    const QFontMetricsF emojiFontMetrics(mEmojiFont);

    const QIcon icon = index.data(EmoticonModel::Icon).value<QIcon>();
    const QString emojiText = index.data(EmoticonModel::UnicodeEmoji).toString();
    if (!icon.isNull()) {
        const int emojiWidth = emojiFontMetrics.horizontalAdvance(QStringLiteral("MM"));
        const QRect displayRect(margin, option.rect.y(), emojiWidth, option.rect.height());
        drawDecoration(painter, option, displayRect, icon.pixmap(emojiWidth, option.rect.height()));
        painter->drawText(margin + emojiWidth, option.rect.y() + emojiFontMetrics.ascent(), emojiText);
    } else {
        const int emojiWidth = emojiFontMetrics.horizontalAdvance(emojiText);
        painter->setFont(mEmojiFont);
        painter->drawText(margin, option.rect.y() + emojiFontMetrics.ascent(), emojiText);
        const QString text = index.data(EmoticonModel::Identifier).toString();
        const int xText = option.rect.x() + margin + emojiWidth;
        const QRect displayRect(xText, option.rect.y(), option.rect.width() - xText, option.rect.height());
        drawDisplay(painter, option, displayRect, text);
    }
}
