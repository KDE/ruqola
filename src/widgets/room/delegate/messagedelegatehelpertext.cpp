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

#include "messagedelegatehelpertext.h"
#include <model/messagemodel.h>
#include "rocketchataccount.h"
#include "ruqola.h"

#include <KLocalizedString>

#include <QPainter>
#include <QTextBlock>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QStyleOptionViewItem>

// TODO: move MessageConvertedText implementation to Message
static QString makeMessageText(const QModelIndex &index)
{
    QString text = index.data(MessageModel::MessageConvertedText).toString();
    const Message::MessageType messageType = index.data(MessageModel::MessageType).value<Message::MessageType>();
    if (messageType == Message::Video) {
        text = i18n("%1 [Video message, not supported yet by ruqola]", text);
    } else if (messageType == Message::Audio) {
        text = i18n("%1 [Audio message, not supported yet by ruqola]", text);
    }
    return text;
}

static bool useItalicsForMessage(const QModelIndex &index)
{
    const Message::MessageType messageType = index.data(MessageModel::MessageType).value<Message::MessageType>();
    const bool isSystemMessage = messageType == Message::System
                                 && index.data(MessageModel::SystemMessageType).toString() != QStringLiteral("jitsi_call_started");
    return isSystemMessage || messageType == Message::Video || messageType == Message::Audio;
}

// QTextDocument lacks a move constructor
static void fillTextDocument(const QModelIndex &index, QTextDocument &doc, const QString &text, int width)
{
    doc.setHtml(text);
    doc.setTextWidth(width);
    if (useItalicsForMessage(index)) {
        QFont font = doc.defaultFont();
        font.setItalic(true);
        doc.setDefaultFont(font);
    }
    QTextFrame *frame = doc.frameAt(0);
    QTextFrameFormat frameFormat = frame->frameFormat();
    frameFormat.setMargin(0);
    frame->setFrameFormat(frameFormat);
}

void MessageDelegateHelperText::draw(QPainter *painter, const QRect &rect, const QModelIndex &index, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(option);
    const QString text = makeMessageText(index);

    if (text.isEmpty()) {
        return;
    }
    // Possible optimisation: store the QTextDocument into the Message itself?
    QTextDocument doc;
    fillTextDocument(index, doc, text, rect.width());
    if (useItalicsForMessage(index)) {
        QTextCursor cursor(&doc);
        cursor.select(QTextCursor::Document);
        QTextCharFormat format;
        format.setForeground(Qt::gray); //TODO use color from theme.
        cursor.mergeCharFormat(format);
    }

    painter->translate(rect.left(), rect.top());
    const QRect clip(0, 0, rect.width(), rect.height());
    doc.drawContents(painter, clip);
    painter->translate(-rect.left(), -rect.top());
}

QSize MessageDelegateHelperText::sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option, qreal *pBaseLine) const
{
    Q_UNUSED(option)
    const QString text = makeMessageText(index);
    if (text.isEmpty()) {
        return QSize();
    }
    QTextDocument doc;
    fillTextDocument(index, doc, text, maxWidth);
    const QSize size(doc.idealWidth(), doc.size().height()); // do the layouting, required by lineAt(0) below

    const QTextLine &line = doc.firstBlock().layout()->lineAt(0);
    *pBaseLine = line.y() + line.ascent(); // relative

    return size;
}

bool MessageDelegateHelperText::handleMouseEvent(QMouseEvent *mouseEvent, const QRect &messageRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        Q_UNUSED(option)
        // ## we should really cache that QTextDocument...
        const QString text = makeMessageText(index);
        QTextDocument doc;
        fillTextDocument(index, doc, text, messageRect.width());

        const QPoint pos = mouseEvent->pos() - messageRect.topLeft();
        const QString link = doc.documentLayout()->anchorAt(pos);
        if (!link.isEmpty()) {
            auto *rcAccount = Ruqola::self()->rocketChatAccount();
            Q_EMIT rcAccount->openLinkRequested(link);
            return true;
        }
    }
    return false;
}
