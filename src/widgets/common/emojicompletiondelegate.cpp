/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojicompletiondelegate.h"
using namespace Qt::Literals::StringLiterals;

#include "common/delegatepaintutil.h"
#include "model/emoticonmodel.h"
#include "utils.h"

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
    const QPen oldPen = painter->pen();
    // The emoji is painted with QPainter::drawText(), so unlike drawDisplay() it doesn't switch
    // to QPalette::HighlightedText by itself.
    DelegatePaintUtil::setTextPen(painter, option);
    const int margin = DelegatePaintUtil::margin();
    const QFontMetricsF emojiFontMetrics(mEmojiFont);

    const QIcon icon = index.data(EmoticonModel::IconRole).value<QIcon>();
    const QString emojiText = index.data(EmoticonModel::UnicodeEmojiRole).toString();
    const int xEmoji = option.rect.x() + margin;
    if (!icon.isNull()) {
        const int emojiWidth = emojiFontMetrics.horizontalAdvance(u"MM"_s);
        const QRect displayRect(xEmoji, option.rect.y(), emojiWidth, option.rect.height());
        // Not drawDecoration(): it centers the pixmap using its device size, so on a HiDPI screen
        // (where QIcon::pixmap() returns a devicePixelRatio > 1 pixmap) it lands outside displayRect.
        const QPixmap pix = icon.pixmap(emojiWidth, option.rect.height());
        const QSizeF pixSize = pix.deviceIndependentSize();
        painter->drawPixmap(
            QPointF(displayRect.x() + (displayRect.width() - pixSize.width()) / 2, displayRect.y() + (displayRect.height() - pixSize.height()) / 2),
            pix);
        painter->drawText(xEmoji + emojiWidth, option.rect.y() + emojiFontMetrics.ascent(), emojiText);
    } else {
        const int emojiWidth = emojiFontMetrics.horizontalAdvance(emojiText);
        painter->setFont(mEmojiFont);
        painter->drawText(xEmoji, option.rect.y() + emojiFontMetrics.ascent(), emojiText);
        const QString text = index.data(EmoticonModel::IdentifierRole).toString();
        const QRect displayRect(xEmoji + emojiWidth, option.rect.y(), option.rect.width() - margin - emojiWidth, option.rect.height());
        drawDisplay(painter, option, displayRect, text);
    }
    painter->setPen(oldPen);
}

#include "moc_emojicompletiondelegate.cpp"
