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
#include "ruqolawidgets_selection_debug.h"
#include "textconverter.h"
#include "utils.h"
#include "messagedelegateutils.h"

#include <KStringHandler>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QClipboard>
#include <QGuiApplication>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QTextDocumentFragment>
#include <QToolTip>
#include <QTextStream>
#include <QTextFrame>
#include <QDrag>
#include <QMimeData>

#include <model/threadmessagemodel.h>

QString MessageDelegateHelperText::makeMessageText(const QModelIndex &index, const QWidget *widget) const
{
    // TODO: move MessageConvertedText implementation to Message?
    QString text = index.data(MessageModel::MessageConvertedText).toString();

    if (mShowThreadContext) {
        const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
        Q_ASSERT(message);
        const QString threadMessageId = message->threadMessageId();
        if (!threadMessageId.isEmpty()) {
            auto *rcAccount = Ruqola::self()->rocketChatAccount();
            const MessageModel *model = rcAccount->messageModelForRoom(message->roomId());
            if (model) {
                auto *that = const_cast<MessageDelegateHelperText *>(this);
                // Find the previous message in the same thread, to use it as context
                auto hasSameThread = [&](const Message &msg) {
                                         return msg.threadMessageId() == threadMessageId
                                                || msg.messageId() == threadMessageId;
                                     };
                Message contextMessage = model->findLastMessageBefore(message->messageId(), hasSameThread);
                if (contextMessage.messageId().isEmpty()) {
                    ThreadMessageModel *cachedModel = mMessageCache.threadMessageModel(threadMessageId);
                    if (cachedModel) {
                        contextMessage = cachedModel->findLastMessageBefore(message->messageId(), hasSameThread);
                        if (contextMessage.messageId().isEmpty()) {
                            Message *msg = mMessageCache.messageForId(threadMessageId);
                            if (msg) {
                                contextMessage = *msg;
                            } else {
                                QPersistentModelIndex persistentIndex(index);
                                connect(&mMessageCache, &MessageCache::messageLoaded,
                                        this, [=](const QString &msgId){
                                    if (msgId == threadMessageId) {
                                        that->updateView(widget, persistentIndex);
                                    }
                                });
                            }
                        } else {
                            //qDebug() << "using cache, found" << contextMessage.messageId() << contextMessage.text();
                        }
                    } else {
                        QPersistentModelIndex persistentIndex(index);
                        connect(&mMessageCache, &MessageCache::modelLoaded,
                                this, [=](){
                            that->updateView(widget, persistentIndex);
                        });
                    }
                }
                // Use TextConverter in case it starts with a [](URL) reply marker
                const QString contextText = KStringHandler::rsqueeze(contextMessage.text(), 200);
                const QString contextString = TextConverter::convertMessageText(contextText, rcAccount->userName(), {}, rcAccount->highlightWords(), rcAccount->emojiManager());
                text.prepend(Utils::formatQuotedRichText(contextString));
            }
        }
    }

    return text;
}

bool MessageDelegateHelperText::hasSelection() const
{
    if (mCurrentTextCursor.isNull()) {
        return false;
    }
    return mCurrentTextCursor.hasSelection();
}

void MessageDelegateHelperText::setCurrentIndex(const QModelIndex &index, const QWidget *view, QRect messageRect)
{
    qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << index.row();
    if (mCurrentIndex.isValid()) {
        // The old index no longer has selection, repaint it
        updateView(view, mCurrentIndex);
    }
    mCurrentIndex = index;
    mCurrentDocument = documentForIndex(index, messageRect.width(), view);
}

void MessageDelegateHelperText::selectAll(const QWidget *view, QRect messageRect, const QModelIndex &index)
{
    if (mCurrentIndex != index) {
        setCurrentIndex(index, view, messageRect);
    }
    if (mCurrentDocument && mCurrentTextCursor.isNull()) {
        mCurrentTextCursor = QTextCursor(mCurrentDocument);
    }
    mCurrentTextCursor.select(QTextCursor::Document);
    updateView(view, mCurrentIndex);
    setClipboardSelection();
}

void MessageDelegateHelperText::removeMessageCache(const QString &messageId)
{
    mDocumentCache.remove(messageId);
}

void MessageDelegateHelperText::clearTextDocumentCache()
{
    //mDocumentCache.clear();
}

QString MessageDelegateHelperText::selectedText() const
{
    if (mCurrentTextCursor.isNull()) {
        return QString();
    }
    const QTextDocumentFragment fragment(mCurrentTextCursor);
    const QString text = fragment.toPlainText();
    qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << text;
    return text;
}

void MessageDelegateHelperText::setClipboardSelection()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "mCurrentTextCursor:" << (mCurrentTextCursor.isNull() ? "null" : "valid");
    if (!mCurrentTextCursor.isNull() && mCurrentTextCursor.hasSelection() && clipboard->supportsSelection()) {
        const QTextDocumentFragment fragment(mCurrentTextCursor);
        const QString text = fragment.toPlainText();
        clipboard->setText(text, QClipboard::Selection);
    }
}

void MessageDelegateHelperText::updateView(const QWidget *widget, const QModelIndex &index)
{
    auto view = qobject_cast<QAbstractItemView *>(const_cast<QWidget *>(widget));
    Q_ASSERT(view);
    view->update(index);
}

static bool useItalicsForMessage(const QModelIndex &index)
{
    const auto messageType = index.data(MessageModel::MessageType).value<Message::MessageType>();
    const bool isSystemMessage = messageType == Message::System
                                 && index.data(MessageModel::SystemMessageType).toString() != QStringLiteral("jitsi_call_started");
    return isSystemMessage;
}

static bool pendingMessage(const QModelIndex &index)
{
    return index.data(MessageModel::PendingMessage).toBool();
}

MessageDelegateHelperText::~MessageDelegateHelperText()
{
}

void MessageDelegateHelperText::draw(QPainter *painter, const QRect &rect, const QModelIndex &index, const QStyleOptionViewItem &option)
{
    auto *doc = documentForIndex(index, rect.width(), option.widget);
    if (!doc) {
        return;
    }

    QVector<QAbstractTextDocumentLayout::Selection> selections;
    if (index == mCurrentIndex) {
        QTextCharFormat selectionFormat;
        selectionFormat.setBackground(option.palette.brush(QPalette::Highlight));
        selectionFormat.setForeground(option.palette.brush(QPalette::HighlightedText));
        selections.append({mCurrentTextCursor, selectionFormat});
    }
    if (useItalicsForMessage(index) || pendingMessage(index)) {
        QTextCursor cursor(doc);
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
    doc->documentLayout()->draw(painter, ctx);
    painter->restore();
}

QSize MessageDelegateHelperText::sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option, qreal *pBaseLine) const
{
    Q_UNUSED(option)
    auto *doc = documentForIndex(index, maxWidth, option.widget);
    if (!doc) {
        return QSize();
    }
    const QSize size(doc->idealWidth(), doc->size().height()); // do the layouting, required by lineAt(0) below

    const QTextLine &line = doc->firstBlock().layout()->lineAt(0);
    *pBaseLine = line.y() + line.ascent(); // relative

    return size;
}

bool MessageDelegateHelperText::handleMouseEvent(QMouseEvent *mouseEvent, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QPoint pos = mouseEvent->pos() - messageRect.topLeft();
    const QEvent::Type eventType = mouseEvent->type();
    // Text selection
    switch (eventType) {
    case QEvent::MouseButtonPress:
        mMightStartDrag = false;
        setCurrentIndex(index, option.widget, messageRect);
        if (mCurrentDocument) {
            const int charPos = mCurrentDocument->documentLayout()->hitTest(pos, Qt::FuzzyHit);
            qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "pressed at pos" << charPos;
            if (charPos == -1) {
                return false;
            }
            if (mCurrentTextCursor.hasSelection()
                && mCurrentTextCursor.selectionStart() <= charPos
                && charPos <= mCurrentTextCursor.selectionEnd()
                && mCurrentDocument->documentLayout()->hitTest(pos, Qt::ExactHit) != -1) {
                mMightStartDrag = true;
                return true;
            }

            // QWidgetTextControl also has code to support selectBlockOnTripleClick, shift to extend selection
            mCurrentTextCursor = QTextCursor(mCurrentDocument);
            mCurrentTextCursor.setPosition(charPos);
            return true;
        } else {
            mCurrentIndex = QModelIndex();
        }
        break;
    case QEvent::MouseMove:
        if (index == mCurrentIndex && mCurrentDocument && !mMightStartDrag) {
            const int charPos = mCurrentDocument->documentLayout()->hitTest(pos, Qt::FuzzyHit);
            if (charPos != -1) {
                // QWidgetTextControl also has code to support isPreediting()/commitPreedit(), selectBlockOnTripleClick
                mCurrentTextCursor.setPosition(charPos, QTextCursor::KeepAnchor);
                return true;
            }
        }
        break;
    case QEvent::MouseButtonRelease:
        qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "released";
        setClipboardSelection();
        // Clicks on links
        if (index == mCurrentIndex && mCurrentDocument && !hasSelection()) {
            const QString link = mCurrentDocument->documentLayout()->anchorAt(pos);
            if (!link.isEmpty()) {
                auto *rcAccount = Ruqola::self()->rocketChatAccount();
                Q_EMIT rcAccount->openLinkRequested(link);
                return true;
            }
        }
        // don't return true here, we need to send mouse release events to other helpers (ex: click on image)
        break;
    case QEvent::MouseButtonDblClick:
        if (index == mCurrentIndex) {
            if (!hasSelection()) {
                mCurrentTextCursor.select(QTextCursor::WordUnderCursor);
                return true;
            }
        }
        break;
    default:
        break;
    }
    return false;
}

bool MessageDelegateHelperText::handleHelpEvent(QHelpEvent *helpEvent, QWidget *view, const QRect &messageRect, const QModelIndex &index)
{
    if (helpEvent->type() != QEvent::ToolTip) {
        return false;
    }

    const auto *doc = documentForIndex(index, messageRect.width(), view);
    if (!doc) {
        return false;
    }

    const QPoint pos = helpEvent->pos() - messageRect.topLeft();
    const auto format = doc->documentLayout()->formatAt(pos);
    const auto tooltip = format.property(QTextFormat::TextToolTip).toString();
    const auto href = format.property(QTextFormat::AnchorHref).toString();
    if (tooltip.isEmpty() && (href.isEmpty() || href.startsWith(QLatin1String("ruqola:/")))) {
        return false;
    }

    QString formattedTooltip;
    QTextStream stream(&formattedTooltip);
    auto addLine = [&](const QString &line) {
                       if (!line.isEmpty()) {
                           stream << QLatin1String("<p>") << line << QLatin1String("</p>");
                       }
                   };

    stream << QLatin1String("<qt>");
    addLine(tooltip);
    addLine(href);
    stream << QLatin1String("</qt>");

    QToolTip::showText(helpEvent->globalPos(), formattedTooltip, view);
    return true;
}

bool MessageDelegateHelperText::maybeStartDrag(QMouseEvent *mouseEvent, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (!mMightStartDrag) {
        return false;
    }
    const QPoint pos = mouseEvent->pos() - messageRect.topLeft();
    if (index == mCurrentIndex && !mCurrentTextCursor.isNull() && mCurrentTextCursor.hasSelection()) {
        const int charPos = mCurrentDocument->documentLayout()->hitTest(pos, Qt::FuzzyHit);
        if (charPos != -1 && mCurrentTextCursor.selectionStart() <= charPos && charPos <= mCurrentTextCursor.selectionEnd()) {
            auto mimeData = new QMimeData;
            const QTextDocumentFragment fragment(mCurrentTextCursor);
            mimeData->setHtml(fragment.toHtml());
            mimeData->setText(fragment.toPlainText());
            auto drag = new QDrag(const_cast<QWidget *>(option.widget));
            drag->setMimeData(mimeData);
            drag->exec(Qt::CopyAction);
            return true;
        }
    }
    return false;
}

void MessageDelegateHelperText::setShowThreadContext(bool b)
{
    mShowThreadContext = b;
}

QTextDocument *MessageDelegateHelperText::documentForIndex(const QModelIndex &index, int width, const QWidget *widget) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    Q_ASSERT(message);
    const auto messageId = message->messageId();
    Q_ASSERT(!messageId.isEmpty());

    auto it = mDocumentCache.find(messageId);
    if (it != mDocumentCache.end()) {
        auto ret = it->value.get();
        if (ret->textWidth() != width) {
            ret->setTextWidth(width);
        }
        return ret;
    }

    const QString text = makeMessageText(index, widget);
    if (text.isEmpty()) {
        return nullptr;
    }

    auto doc = MessageDelegateUtils::createTextDocument(useItalicsForMessage(index), text, width);
    auto ret = doc.get();
    mDocumentCache.insert(messageId, std::move(doc));
    return ret;
}
