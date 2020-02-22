/*
   Copyright (c) 2020 David Faure <faure@kde.org>

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

#include "emojicompletiondelegate.h"

#include <model/emoticonmodel.h>

#include <QPainter>

EmojiCompletionDelegate::EmojiCompletionDelegate(QObject *parent)
    : QItemDelegate(parent),
     mEmojiFont(QStringLiteral("NotoColorEmoji"))
{
}

void EmojiCompletionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] <unicode emoji> <:identifier:>     ([M] = margin)
    drawBackground(painter, option, index);
    const int margin = 8;
    QFontMetricsF emojiFontMetrics(mEmojiFont);
    const QString emojiText = index.data(EmoticonModel::UnicodeEmoji).toString();
    const int emojiWidth = emojiFontMetrics.horizontalAdvance(emojiText);
    painter->setFont(mEmojiFont);
    painter->drawText(margin, option.rect.y() + emojiFontMetrics.ascent(), emojiText);

    const QString text = index.data(Qt::DisplayRole).toString();
    const int xText = option.rect.x() + margin + emojiWidth;
    const QRect displayRect(xText, option.rect.y(),
                            option.rect.width() - xText,
                            option.rect.height());

    drawDisplay(painter, option, displayRect, text);
}
