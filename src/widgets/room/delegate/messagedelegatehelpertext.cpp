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
#include "textconverter.h"

#include <KLocalizedString>
#include <KStringHandler>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QClipboard>
#include <QGuiApplication>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QTextBlock>
#include <QTextDocumentFragment>

QString MessageDelegateHelperText::makeMessageText(const QModelIndex &index) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    Q_ASSERT(message);

    // TODO: move MessageConvertedText implementation to Message?
    QString text = index.data(MessageModel::MessageConvertedText).toString();
    const Message::MessageType messageType = message->messageType();
    if (messageType == Message::Video) {
        text = i18n("%1 [Video message, not supported yet by ruqola]", text);
    } else if (messageType == Message::Audio) {
        text = i18n("%1 [Audio message, not supported yet by ruqola]", text);
    }

    if (mShowThreadContext) {
        const QString threadMessageId = message->threadMessageId();
        if (!threadMessageId.isEmpty()) {
            auto *rcAccount = Ruqola::self()->rocketChatAccount();
            const MessageModel *model = rcAccount->messageModelForRoom(message->roomId());
            // Find the previous message in the same thread, to use it as context
            auto hasSameThread = [&](const Message &msg) {
                return msg.threadMessageId() == threadMessageId ||
                        msg.messageId() == threadMessageId;
            };
            const Message contextMessage = model->findLastMessageBefore(message->messageId(), hasSameThread);
            // Use TextConverter in case it starts with a [](URL) reply marker
            TextConverter textConverter(rcAccount->emojiManager());
            QString contextString = textConverter.convertMessageText(contextMessage.text(), rcAccount->userName(), {});
            contextString = KStringHandler::rsqueeze(contextString, 200);
            text.prepend(QStringLiteral("<font size=\"-1\">&gt; %1</font><br/>").arg(contextString));
        }
    }

    return text;
}

void MessageDelegateHelperText::setClipboardSelection()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    if (mCurrentTextCursor.hasSelection() && clipboard->supportsSelection()) {
        const QTextDocumentFragment fragment(mCurrentTextCursor);
        const QString text = fragment.toPlainText();
        clipboard->setText(text, QClipboard::Selection);
    }
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

void MessageDelegateHelperText::draw(QPainter *painter, const QRect &rect, const QModelIndex &index, const QStyleOptionViewItem &option)
{
    const QString text = makeMessageText(index);

    if (text.isEmpty()) {
        return;
    }
    // Possible optimisation: store the QTextDocument into the Message itself?
    QTextDocument doc;
    QTextDocument *pDoc = &doc;
    QVector<QAbstractTextDocumentLayout::Selection> selections;
    if (index == mCurrentIndex) {
        pDoc = &mCurrentDocument; // optimization, not stricly necessary
        QTextCharFormat selectionFormat;
        selectionFormat.setBackground(option.palette.brush(QPalette::Highlight));
        selectionFormat.setForeground(option.palette.brush(QPalette::HighlightedText));
        selections.append({mCurrentTextCursor, selectionFormat});
    } else {
        fillTextDocument(index, doc, text, rect.width());
    }
    if (useItalicsForMessage(index)) {
        QTextCursor cursor(pDoc);
        cursor.select(QTextCursor::Document);
        QTextCharFormat format;
        format.setForeground(Qt::gray); //TODO use color from theme.
        cursor.mergeCharFormat(format);
    }

    painter->save();
    painter->translate(rect.left(), rect.top());
    const QRect clip(0, 0, rect.width(), rect.height());

    // Same as pDoc->drawContents(painter, clip) but we also set selections
    QAbstractTextDocumentLayout::PaintContext ctx;
    ctx.selections = selections;
    if (clip.isValid()) {
        painter->setClipRect(clip);
        ctx.clip = clip;
    }
    pDoc->documentLayout()->draw(painter, ctx);
    painter->restore();
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
    const QPoint pos = mouseEvent->pos() - messageRect.topLeft();
    const QEvent::Type eventType = mouseEvent->type();
    // Text selection
    switch (eventType) {
    case QEvent::MouseButtonPress: {
        if (mCurrentIndex.isValid()) {
            // The old index no longer has selection, repaint it
            auto *view = qobject_cast<QAbstractItemView *>(const_cast<QWidget *>(option.widget));
            Q_ASSERT(view);
            view->update(mCurrentIndex);
        }
        mCurrentIndex = index;
        const QString text = makeMessageText(index);
        fillTextDocument(index, mCurrentDocument, text, messageRect.width());
        const int charPos = mCurrentDocument.documentLayout()->hitTest(pos, Qt::FuzzyHit);
        // QWidgetTextControl also has code to support selectBlockOnTripleClick, shift to extend selection
        if (charPos != -1) {
            mCurrentTextCursor = QTextCursor(&mCurrentDocument);
            mCurrentTextCursor.setPosition(charPos);
            return true;
        }
        break;
    }
    case QEvent::MouseMove: {
        if (index == mCurrentIndex) {
            const int charPos = mCurrentDocument.documentLayout()->hitTest(pos, Qt::FuzzyHit);
            if (charPos != -1) {
                // QWidgetTextControl also has code to support dragging, isPreediting()/commitPreedit(), selectBlockOnTripleClick
                mCurrentTextCursor.setPosition(charPos, QTextCursor::KeepAnchor);
                return true;
            }
        }
        break;
    }
    case QEvent::MouseButtonRelease: {
        if (index == mCurrentIndex) {
            setClipboardSelection();
        }
        break;
    }
    case QEvent::MouseButtonDblClick: {
        if (index == mCurrentIndex) {
            if (!mCurrentTextCursor.hasSelection()) {
                mCurrentTextCursor.select(QTextCursor::WordUnderCursor);
                // Interestingly the view repaints after mouse press, mouse move and mouse release
                // but not after double-click, so make it happen:
                auto *view = qobject_cast<QAbstractItemView *>(const_cast<QWidget *>(option.widget));
                Q_ASSERT(view);
                view->update(mCurrentIndex);
                setClipboardSelection();
            }
        }
        break;
    }
    default:
        break;
    }
    // Clicks on links
    if (eventType == QEvent::MouseButtonRelease) {
        // ## we should really cache that QTextDocument...
        const QString text = makeMessageText(index);
        QTextDocument doc;
        fillTextDocument(index, doc, text, messageRect.width());

        const QString link = doc.documentLayout()->anchorAt(pos);
        if (!link.isEmpty()) {
            auto *rcAccount = Ruqola::self()->rocketChatAccount();
            Q_EMIT rcAccount->openLinkRequested(link);
            return true;
        }
    }
    return false;
}

void MessageDelegateHelperText::setShowThreadContext(bool b)
{
    mShowThreadContext = b;
}
