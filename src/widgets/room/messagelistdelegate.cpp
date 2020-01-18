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

#include "messagelistdelegate.h"
#include "model/messagemodel.h"

#include <QPainter>
#include <QStyle>
#include <QTextBlock>
#include <QTextDocument>

MessageListDelegate::MessageListDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

static void drawRichText(QPainter *painter, const QRect &rect, const QString &text, qreal *pBaseLine)
{
    // Possible optimisation: store the QTextDocument into the Message itself?
    QTextDocument doc;
    doc.setHtml(text);
    doc.setTextWidth(rect.width());

    //QStyleOptionViewItemV4 options = option;
    //initStyleOption(&options, index);
    //options.text = QString();
    //options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

    painter->translate(rect.left(), rect.top());
    const QRect clip(0, 0, rect.width(), rect.height());
    doc.drawContents(painter, clip);
    painter->translate(-rect.left(), -rect.top());

    const qreal frameMargin = doc.frameAt(0)->frameFormat().topMargin();
    const QTextLine &line = doc.firstBlock().layout()->lineAt(0);
    *pBaseLine = frameMargin + line.y() + line.ascent();
}

static QString makeSenderText(const QString &sender)
{
    return QLatin1Char('@') + sender;
}

static QString makeTimeStampText(const QModelIndex &index)
{
    return index.data(MessageModel::Timestamp).toString();
}

static void drawTimestamp(const QModelIndex &index, QPainter *painter, const QStyleOptionViewItem &option, QRect *timeRect)
{
    const QString timeStampText = makeTimeStampText(index);
    // This gives incorrect results (too small bounding rect), no idea why!
    //const QSize timeSize = painter->fontMetrics().boundingRect(timeStampText).size();
    const QSize timeSize(option.fontMetrics.horizontalAdvance(timeStampText), option.fontMetrics.height());

    *timeRect = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignRight | Qt::AlignVCenter, timeSize, option.rect);
    const QPen oldPen = painter->pen();
    QColor col = painter->pen().color();
    col.setAlpha(128); // TimestampText.qml had opacity: .5
    painter->setPen(col);
    painter->drawText(*timeRect, timeStampText);
    painter->setPen(oldPen);

    painter->setPen(oldPen);
}

void MessageListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    drawBackground(painter, option, index);

    // Compact mode : <icon> <sender> <message> <smiley> <timestamp>


    //QRect decorationRect = opt.rect;
    const QString sender = index.data(MessageModel::Username).toString();
#if 0
    const QString iconFileName = mCache->avatarUrlFromCacheOnly(sender);
    QPixmap pixmap;
    if (!iconFileName.isEmpty()) {
        const bool loaded = pix.load(iconFileName); // SLOW! QPixmapCache anyone?
    }

    drawDecoration(painter, opt, decorationRect, pixmap);
#endif

    // Sender (calculate size, but don't draw it yet, we need to align vertically to the first line of the message)
    const QString senderText = makeSenderText(sender);
    QFont boldFont = option.font;
    boldFont.setBold(true);
    const QFontMetrics senderFontMetrics(boldFont);
    QRect senderRect = option.rect; //  for now
    senderRect.setSize(senderFontMetrics.boundingRect(senderText).size());

    // Timestamp
    QRect timeRect;
    drawTimestamp(index, painter, option, &timeRect);

    // Message
    QRect messageRect = option.rect;
    messageRect.setLeft(senderRect.right());
    messageRect.setRight(timeRect.left() - 1);
    const QString message = index.data(MessageModel::MessageConvertedText).toString();

    qreal baseLine = 0;
    drawRichText(painter, messageRect, message, &baseLine);

    // Now draw the sender
    painter->setFont(boldFont);
    painter->drawText(senderRect.x(), senderRect.y() + baseLine, senderText);
    painter->setFont(option.font);

    //drawFocus(painter, option, displayRect);
}

QSize MessageListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Sender
    const QString sender = index.data(MessageModel::Username).toString();
    QFont boldFont = option.font;
    boldFont.setBold(true);
    QRect senderRect = option.rect; //  for now
    const QFontMetrics senderFontMetrics(boldFont);
    senderRect.setSize(senderFontMetrics.boundingRect(makeSenderText(sender)).size());

    // Timestamp
    const QString timeStampText = makeTimeStampText(index);
    const QSize timeSize = option.fontMetrics.boundingRect(timeStampText).size();

    // Message (using the rest of the available width)
    QTextDocument doc;
    const QString message = index.data(MessageModel::MessageConvertedText).toString();
    doc.setHtml(message);
    doc.setTextWidth(option.rect.width() - senderRect.width() - timeSize.width());

    return QSize(senderRect.width() + doc.idealWidth() + timeSize.width(),
                 qMax<int>(senderRect.height(), doc.size().height()));
}
