/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "emojicustomdelegate.h"
#include "common/delegatepaintutil.h"
#include "utils.h"
#include <model/emoticonmodel.h>

#include <QPainter>

EmojiCustomDelegate::EmojiCustomDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mEmojiFont(Utils::emojiFontName())
{
}

EmojiCustomDelegate::~EmojiCustomDelegate() = default;

void EmojiCustomDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    drawBackground(painter, option, index);
    const int margin = DelegatePaintUtil::margin();
    // TODO Use Icon for custom emoji
    const QFontMetricsF emojiFontMetrics(mEmojiFont);
    const QString emojiText = index.data(EmoticonModel::UnicodeEmoji).toString();
    const int emojiWidth = emojiFontMetrics.horizontalAdvance(emojiText);
    painter->setFont(mEmojiFont);
    painter->drawText(margin, option.rect.y() + emojiFontMetrics.ascent(), emojiText);

    const QString text = index.data(EmoticonModel::CompleterName).toString();
    const int xText = option.rect.x() + margin + emojiWidth;
    const QRect displayRect(xText, option.rect.y(), option.rect.width() - xText, option.rect.height());

    drawDisplay(painter, option, displayRect, text);
}
