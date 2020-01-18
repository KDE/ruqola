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
#include <QTextDocument>

MessageListDelegate::MessageListDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

static void drawRichText(QPainter *painter, const QRect &rect, const QString &text)
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
}

static QString makeSenderText(const QString &sender)
{
    return QLatin1Char('@') + sender;
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

    // Sender
    const QString senderText = makeSenderText(sender);
    const QFont font = option.font;
    QFont boldFont = font;
    boldFont.setBold(true);
    painter->setFont(boldFont);
    const QFontMetrics senderFontMetrics(boldFont);
    QRect senderRect = option.rect; //  for now
    senderRect.setSize(senderFontMetrics.boundingRect(senderText).size());
    painter->drawText(senderRect, senderText);
    painter->setFont(font);

    // Message
    QRect messageRect = option.rect;
    messageRect.setLeft(senderRect.right());
    const QString message = index.data(MessageModel::MessageConvertedText).toString();

    drawRichText(painter, messageRect, message);

    //drawFocus(painter, option, displayRect);
}

QSize MessageListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QString sender = index.data(MessageModel::Username).toString();
    QFont boldFont = option.font;
    boldFont.setBold(true);
    QRect senderRect = option.rect; //  for now
    const QFontMetrics senderFontMetrics(boldFont);
    senderRect.setSize(senderFontMetrics.boundingRect(makeSenderText(sender)).size());

    QTextDocument doc;
    const QString message = index.data(MessageModel::MessageConvertedText).toString();
    doc.setHtml(message);
    doc.setTextWidth(option.rect.width());
    return QSize(senderRect.width() + doc.idealWidth(), qMax<int>(senderRect.height(), doc.size().height()));
}
