/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    Q_ASSERT(message);
    const QString threadMessageId = message->threadMessageId();

    if (mShowThreadContext && !threadMessageId.isEmpty()) {
        const auto sameAsPreviousMessageThread = [&] {
            if (index.row() < 1) {
                return false;
            }
            const auto previousIndex = index.siblingAtRow(index.row() - 1);
            const auto *previousMessage = previousIndex.data(MessageModel::MessagePointer).value<Message *>();
            Q_ASSERT(previousMessage);
            return threadMessageId == previousMessage->threadMessageId();
        }();
        if (!sameAsPreviousMessageThread) {
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
                                                                                needUpdateMessageId,
                                                                                contextMessage.mentions(),
                                                                                contextMessage.channels());
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

void MessageDelegateHelperText::selectAll(const QModelIndex &index)
{
    mSelection->selectMessage(index, this);
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

QString MessageDelegateHelperText::selectedText()
{
    const QString text = mSelection->selectedText(TextSelection::Text, this);
    // TODO fix selectedText when we are attachment
    qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "selected text : " << text;
    return text;
}

QString MessageDelegateHelperText::urlAt(const QModelIndex &index, QPoint relativePos) const
{
    auto document = documentForIndex(index);
    if (!document) {
        return {};
    }

    return document->documentLayout()->anchorAt(relativePos);
}

void MessageDelegateHelperText::setClipboardSelection()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    if (mSelection->hasSelection() && clipboard->supportsSelection()) {
        const QString text = mSelection->selectedText(TextSelection::Text, this);
        clipboard->setText(text, QClipboard::Selection);
    }
}

void MessageDelegateHelperText::updateView(const QModelIndex &index)
{
    // qDebug() << " void MessageDelegateHelperText::updateView(const QModelIndex &index)" << index;
    mListView->update(index);
}

MessageDelegateHelperText::MessageDelegateHelperText(QListView *view, TextSelection *textSelection)
    : QObject(view)
    , mListView(view)
    , mSelection(textSelection)
{
    connect(mSelection, &TextSelection::repaintNeeded, this, &MessageDelegateHelperText::updateView);
}

MessageDelegateHelperText::~MessageDelegateHelperText() = default;

void MessageDelegateHelperText::draw(QPainter *painter, QRect rect, const QModelIndex &index, const QStyleOptionViewItem &option)
{
    auto *doc = documentForIndex(index, rect.width(), true);
    if (!doc) {
        return;
    }

    MessageDelegateUtils::drawSelection(doc, rect, rect.top(), painter, index, option, mSelection);
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

bool MessageDelegateHelperText::handleMouseEvent(QMouseEvent *mouseEvent, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index)
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
            if (mSelection->contains(index, charPos) && doc->documentLayout()->hitTest(pos, Qt::ExactHit) != -1) {
                mMightStartDrag = true;
                return true;
            }

            // QWidgetTextControl also has code to support selectBlockOnTripleClick, shift to extend selection
            // (look there if you want to add these things)

            mSelection->setStart(index, charPos);
            return true;
        } else {
            mSelection->clear();
        }
        break;
    case QEvent::MouseMove:
        if (!mMightStartDrag) {
            if (const auto *doc = documentForIndex(index, messageRect.width(), true)) {
                const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
                if (charPos != -1) {
                    // QWidgetTextControl also has code to support isPreediting()/commitPreedit(), selectBlockOnTripleClick
                    mSelection->setEnd(index, charPos);
                    return true;
                }
            }
        }
        break;
    case QEvent::MouseButtonRelease:
        qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "released";
        setClipboardSelection();
        // Clicks on links
        if (!mSelection->hasSelection()) {
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
            mSelection->clear();
        }
        // don't return true here, we need to send mouse release events to other helpers (ex: click on image)
        break;
    case QEvent::MouseButtonDblClick:
        if (!mSelection->hasSelection()) {
            if (const auto *doc = documentForIndex(index, messageRect.width(), true)) {
                const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
                qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "double-clicked at pos" << charPos;
                if (charPos == -1) {
                    return false;
                }
                mSelection->selectWordUnderCursor(index, charPos, this);
                return true;
            }
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
    QString formattedTooltip;
    if (MessageDelegateUtils::generateToolTip(doc, pos, formattedTooltip)) {
        QToolTip::showText(helpEvent->globalPos(), formattedTooltip, mListView);
        return true;
    }
    return true;
}

bool MessageDelegateHelperText::maybeStartDrag(QMouseEvent *mouseEvent, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (!mMightStartDrag) {
        return false;
    }
    const QPoint pos = mouseEvent->pos() - messageRect.topLeft();
    if (mSelection->hasSelection()) {
        const auto *doc = documentForIndex(index, messageRect.width(), false);
        const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
        if (charPos != -1 && mSelection->contains(index, charPos)) {
            auto mimeData = new QMimeData;
            mimeData->setHtml(mSelection->selectedText(TextSelection::Html, this));
            mimeData->setText(mSelection->selectedText(TextSelection::Text, this));
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

QTextDocument *MessageDelegateHelperText::documentForIndex(const MessageAttachment &msgAttach) const
{
    Q_UNUSED(msgAttach);
    Q_ASSERT(false);
    // Unused here
    return nullptr;
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

    auto doc = MessageDelegateUtils::createTextDocument(MessageDelegateUtils::useItalicsForMessage(index), text, width);
    auto ret = doc.get();
    mDocumentCache.insert(messageId, std::move(doc));
    return ret;
}
