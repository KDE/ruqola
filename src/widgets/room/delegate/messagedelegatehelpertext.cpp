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
#include "messagedelegateutils.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_selection_debug.h"
#include "textconverter.h"
#include "utils.h"
#include <messagecache.h>
#include <model/messagemodel.h>
#include <model/threadmessagemodel.h>

#include <KStringHandler>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QClipboard>
#include <QDrag>
#include <QGuiApplication>
#include <QListView>
#include <QMimeData>
#include <QPainter>
#include <QScreen>
#include <QStyleOptionViewItem>
#include <QTextBlock>
#include <QTextDocumentFragment>
#include <QTextStream>
#include <QToolTip>

QString MessageDelegateHelperText::makeMessageText(const QModelIndex &index, bool connectToUpdates) const
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
                    return msg.threadMessageId() == threadMessageId || msg.messageId() == threadMessageId;
                };
                Message contextMessage = model->findLastMessageBefore(message->messageId(), hasSameThread);
                auto messageCache = rcAccount->messageCache();
                if (contextMessage.messageId().isEmpty()) {
                    ThreadMessageModel *cachedModel = messageCache->threadMessageModel(threadMessageId);
                    if (cachedModel) {
                        contextMessage = cachedModel->findLastMessageBefore(message->messageId(), hasSameThread);
                        if (contextMessage.messageId().isEmpty()) {
                            Message *msg = messageCache->messageForId(threadMessageId);
                            if (msg) {
                                contextMessage = *msg;
                            } else if (connectToUpdates) {
                                QPersistentModelIndex persistentIndex(index);
                                connect(messageCache, &MessageCache::messageLoaded, this, [=](const QString &msgId) {
                                    if (msgId == threadMessageId) {
                                        that->updateView(persistentIndex);
                                    }
                                });
                            }
                        } else {
                            // qDebug() << "using cache, found" << contextMessage.messageId() << contextMessage.text();
                        }
                    } else if (connectToUpdates) {
                        QPersistentModelIndex persistentIndex(index);
                        connect(messageCache, &MessageCache::modelLoaded, this, [=]() {
                            that->updateView(persistentIndex);
                        });
                    }
                }
                // Use TextConverter in case it starts with a [](URL) reply marker
                const QString contextText =
                    KStringHandler::rsqueeze(QLatin1Char('@') + contextMessage.username() + QLatin1String(": ") + contextMessage.text(), 200);

                QString needUpdateMessageId;
                const QString contextString = TextConverter::convertMessageText(contextText,
                                                                                rcAccount->userName(),
                                                                                {},
                                                                                rcAccount->highlightWords(),
                                                                                rcAccount->emojiManager(),
                                                                                rcAccount->messageCache(),
                                                                                needUpdateMessageId);
                if (!needUpdateMessageId.isEmpty() && connectToUpdates) {
                    QPersistentModelIndex persistentIndex(index);
                    connect(messageCache, &MessageCache::messageLoaded, this, [=](const QString &msgId) {
                        if (msgId == needUpdateMessageId) {
                            that->updateView(persistentIndex);
                        }
                    });
                }
                text.prepend(Utils::formatQuotedRichText(contextString));
            }
        }
    }

    return text;
}

bool MessageDelegateHelperText::hasSelection() const
{
    return mSelection.hasSelection();
}

void MessageDelegateHelperText::selectAll(const QModelIndex &index)
{
    mSelection.selectMessage(index);
    updateView(index);
    setClipboardSelection();
}

void MessageDelegateHelperText::removeMessageCache(const QString &messageId)
{
    mDocumentCache.remove(messageId);
}

void MessageDelegateHelperText::clearTextDocumentCache()
{
    mDocumentCache.clear();
}

QString MessageDelegateHelperText::selectedText() const
{
    const QString text = mSelection.selectedText(TextSelection::Text);
    qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "selected text : " << text;
    return text;
}

void MessageDelegateHelperText::clearSelection()
{
    mSelection.clear();
}

void MessageDelegateHelperText::setClipboardSelection()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    if (mSelection.hasSelection() && clipboard->supportsSelection()) {
        const QString text = mSelection.selectedText(TextSelection::Text);
        clipboard->setText(text, QClipboard::Selection);
    }
}

void MessageDelegateHelperText::updateView(const QModelIndex &index)
{
    mListView->update(index);
}

static bool useItalicsForMessage(const QModelIndex &index)
{
    const auto messageType = index.data(MessageModel::MessageType).value<Message::MessageType>();
    const bool isSystemMessage =
        messageType == Message::System && index.data(MessageModel::SystemMessageType).toString() != QStringLiteral("jitsi_call_started");
    return isSystemMessage;
}

static bool pendingMessage(const QModelIndex &index)
{
    return index.data(MessageModel::PendingMessage).toBool();
}

MessageDelegateHelperText::MessageDelegateHelperText(QListView *view)
    : QObject(view)
    , mListView(view)
    , mSelection(this)
{
    connect(&mSelection, &TextSelection::repaintNeeded, this, &MessageDelegateHelperText::updateView);
}

MessageDelegateHelperText::~MessageDelegateHelperText() = default;

void MessageDelegateHelperText::draw(QPainter *painter, QRect rect, const QModelIndex &index, const QStyleOptionViewItem &option)
{
    auto *doc = documentForIndex(index, rect.width(), true);
    if (!doc) {
        return;
    }

    QVector<QAbstractTextDocumentLayout::Selection> selections;
    const QTextCursor selectionTextCursor = mSelection.selectionForIndex(index, doc);
    if (!selectionTextCursor.isNull()) {
        QTextCharFormat selectionFormat;
        selectionFormat.setBackground(option.palette.brush(QPalette::Highlight));
        selectionFormat.setForeground(option.palette.brush(QPalette::HighlightedText));
        selections.append({selectionTextCursor, selectionFormat});
    }
    if (useItalicsForMessage(index) || pendingMessage(index)) {
        QTextCursor cursor(doc);
        cursor.select(QTextCursor::Document);
        QTextCharFormat format;
        format.setForeground(Qt::gray); // TODO use color from theme.
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
    auto *doc = documentForIndex(index, maxWidth, true);
    if (!doc) {
        return {};
    }
    const QSize size(doc->idealWidth(), doc->size().height()); // do the layouting, required by lineAt(0) below

    const QTextLine &line = doc->firstBlock().layout()->lineAt(0);
    *pBaseLine = line.y() + line.ascent(); // relative

    return size;
}

bool MessageDelegateHelperText::handleMouseEvent(QMouseEvent *mouseEvent,
                                                 const QRect &messageRect,
                                                 const QStyleOptionViewItem &option,
                                                 const QModelIndex &index)
{
    Q_UNUSED(option)
    if (!messageRect.contains(mouseEvent->pos())) {
        return false;
    }

    const QPoint pos = mouseEvent->pos() - messageRect.topLeft();
    const QEvent::Type eventType = mouseEvent->type();
    // Text selection
    switch (eventType) {
    case QEvent::MouseButtonPress:
        mMightStartDrag = false;
        if (const auto *doc = documentForIndex(index, messageRect.width(), true)) {
            const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
            qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "pressed at pos" << charPos;
            if (charPos == -1) {
                return false;
            }
            if (mSelection.contains(index, charPos) && doc->documentLayout()->hitTest(pos, Qt::ExactHit) != -1) {
                mMightStartDrag = true;
                return true;
            }

            // QWidgetTextControl also has code to support selectBlockOnTripleClick, shift to extend selection
            // (look there if you want to add these things)

            mSelection.setStart(index, charPos);
            return true;
        } else {
            mSelection.clear();
        }
        break;
    case QEvent::MouseMove:
        if (!mMightStartDrag) {
            if (const auto *doc = documentForIndex(index, messageRect.width(), true)) {
                const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
                if (charPos != -1) {
                    // QWidgetTextControl also has code to support isPreediting()/commitPreedit(), selectBlockOnTripleClick
                    mSelection.setEnd(index, charPos);
                    return true;
                }
            }
        }
        break;
    case QEvent::MouseButtonRelease:
        qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "released";
        setClipboardSelection();
        // Clicks on links
        if (!mSelection.hasSelection()) {
            if (const auto *doc = documentForIndex(index, messageRect.width(), true)) {
                const QString link = doc->documentLayout()->anchorAt(pos);
                if (!link.isEmpty()) {
                    auto *rcAccount = Ruqola::self()->rocketChatAccount();
                    Q_EMIT rcAccount->openLinkRequested(link);
                    return true;
                }
            }
        } else if (mMightStartDrag) {
            // clicked into selection, didn't start drag, clear it (like kwrite and QTextEdit)
            mSelection.clear();
        }
        // don't return true here, we need to send mouse release events to other helpers (ex: click on image)
        break;
    case QEvent::MouseButtonDblClick:
        if (!mSelection.hasSelection()) {
            mSelection.selectWordUnderCursor();
            return true;
        }
        break;
    default:
        break;
    }
    return false;
}

bool MessageDelegateHelperText::handleHelpEvent(QHelpEvent *helpEvent, QRect messageRect, const QModelIndex &index)
{
    if (helpEvent->type() != QEvent::ToolTip) {
        return false;
    }

    const auto *doc = documentForIndex(index, messageRect.width(), true);
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

    QToolTip::showText(helpEvent->globalPos(), formattedTooltip, mListView);
    return true;
}

bool MessageDelegateHelperText::maybeStartDrag(QMouseEvent *mouseEvent, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (!mMightStartDrag) {
        return false;
    }
    const QPoint pos = mouseEvent->pos() - messageRect.topLeft();
    if (mSelection.hasSelection()) {
        const auto *doc = documentForIndex(index, messageRect.width(), false);
        const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
        if (charPos != -1 && mSelection.contains(index, charPos)) {
            auto mimeData = new QMimeData;
            mimeData->setHtml(mSelection.selectedText(TextSelection::Html));
            mimeData->setText(mSelection.selectedText(TextSelection::Text));
            auto drag = new QDrag(const_cast<QWidget *>(option.widget));
            drag->setMimeData(mimeData);
            drag->exec(Qt::CopyAction);
            mMightStartDrag = false; // don't clear selection on release
            return true;
        }
    }
    return false;
}

void MessageDelegateHelperText::setShowThreadContext(bool b)
{
    mShowThreadContext = b;
}

QTextDocument *MessageDelegateHelperText::documentForIndex(const QModelIndex &index) const
{
    return documentForIndex(index, -1, false);
}

QTextDocument *MessageDelegateHelperText::documentForIndex(const QModelIndex &index, int width, bool connectToUpdates) const
{
    Q_ASSERT(index.isValid());
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    Q_ASSERT(message);
    const auto messageId = message->messageId();
    Q_ASSERT(!messageId.isEmpty());

    auto it = mDocumentCache.find(messageId);
    if (it != mDocumentCache.end()) {
        auto ret = it->value.get();
        if (width != -1 && !qFuzzyCompare(ret->textWidth(), width)) {
            ret->setTextWidth(width);
        }
        return ret;
    }

    const QString text = makeMessageText(index, connectToUpdates);
    if (text.isEmpty()) {
        return nullptr;
    }

    auto doc = MessageDelegateUtils::createTextDocument(useItalicsForMessage(index), text, width);
    auto ret = doc.get();
    mDocumentCache.insert(messageId, std::move(doc));
    return ret;
}
