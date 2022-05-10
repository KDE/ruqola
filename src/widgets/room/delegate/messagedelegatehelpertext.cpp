/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelpertext.h"
#include "delegateutils/messagedelegateutils.h"
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
#include <QDrag>
#include <QListView>
#include <QMimeData>
#include <QPainter>
#include <QStyleOptionViewItem>
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
                                                                                contextMessage.channels(),
                                                                                mSearchText);
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

void MessageDelegateHelperText::removeMessageCache(const QString &messageId)
{
    mDocumentCache.remove(messageId);
}

void MessageDelegateHelperText::clearTextDocumentCache()
{
    mDocumentCache.clear();
}

void MessageDelegateHelperText::setSearchText(const QString &newSearchText)
{
    mSearchText = newSearchText;
}

QString MessageDelegateHelperText::urlAt(const QModelIndex &index, QPoint relativePos) const
{
    auto document = documentForIndex(index);
    if (!document) {
        return {};
    }

    return document->documentLayout()->anchorAt(relativePos);
}

void MessageDelegateHelperText::updateView(const QModelIndex &index)
{
    // qDebug() << " void MessageDelegateHelperText::updateView(const QModelIndex &index)" << index;
    mListView->update(index);
}

MessageDelegateHelperText::MessageDelegateHelperText(QListView *view, TextSelectionImpl *textSelectionImpl)
    : QObject(view)
    , mListView(view)
    , mSelectionImpl(textSelectionImpl)
{
    connect(mSelectionImpl->textSelection(), &TextSelection::repaintNeeded, this, &MessageDelegateHelperText::updateView);
}

MessageDelegateHelperText::~MessageDelegateHelperText() = default;

void MessageDelegateHelperText::draw(QPainter *painter, QRect rect, const QModelIndex &index, const QStyleOptionViewItem &option)
{
    auto *doc = documentForIndex(index, rect.width(), true);
    if (!doc) {
        return;
    }

    MessageDelegateUtils::drawSelection(doc, rect, rect.top(), painter, index, option, mSelectionImpl->textSelection(), {});
}

QSize MessageDelegateHelperText::sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option, qreal *pBaseLine) const
{
    Q_UNUSED(option)
    auto *doc = documentForIndex(index, maxWidth, true);
    return MessageDelegateUtils::textSizeHint(doc, pBaseLine);
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
        mSelectionImpl->setMightStartDrag(false);
        if (const auto *doc = documentForIndex(index, messageRect.width(), true)) {
            const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
            qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "pressed at pos" << charPos;
            if (charPos == -1) {
                return false;
            }
            if (mSelectionImpl->textSelection()->contains(index, charPos) && doc->documentLayout()->hitTest(pos, Qt::ExactHit) != -1) {
                mSelectionImpl->setMightStartDrag(true);
                return true;
            }

            // QWidgetTextControl also has code to support selectBlockOnTripleClick, shift to extend selection
            // (look there if you want to add these things)

            mSelectionImpl->textSelection()->setStart(index, charPos);
            return true;
        } else {
            mSelectionImpl->textSelection()->clear();
        }
        break;
    case QEvent::MouseMove:
        if (!mSelectionImpl->mightStartDrag()) {
            if (const auto *doc = documentForIndex(index, messageRect.width(), true)) {
                const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
                if (charPos != -1) {
                    // QWidgetTextControl also has code to support isPreediting()/commitPreedit(), selectBlockOnTripleClick
                    mSelectionImpl->textSelection()->setEnd(index, charPos);
                    return true;
                }
            }
        }
        break;
    case QEvent::MouseButtonRelease:
        qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "released";
        MessageDelegateUtils::setClipboardSelection(mSelectionImpl->textSelection());
        // Clicks on links
        if (!mSelectionImpl->textSelection()->hasSelection()) {
            if (const auto *doc = documentForIndex(index, messageRect.width(), true)) {
                const QString link = doc->documentLayout()->anchorAt(pos);
                if (!link.isEmpty()) {
                    auto *rcAccount = Ruqola::self()->rocketChatAccount();
                    Q_EMIT rcAccount->openLinkRequested(link);
                    return true;
                }
            }
        } else if (mSelectionImpl->mightStartDrag()) {
            // clicked into selection, didn't start drag, clear it (like kwrite and QTextEdit)
            mSelectionImpl->textSelection()->clear();
        }
        // don't return true here, we need to send mouse release events to other helpers (ex: click on image)
        break;
    case QEvent::MouseButtonDblClick:
        if (!mSelectionImpl->textSelection()->hasSelection()) {
            if (const auto *doc = documentForIndex(index, messageRect.width(), true)) {
                const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
                qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "double-clicked at pos" << charPos;
                if (charPos == -1) {
                    return false;
                }
                mSelectionImpl->textSelection()->selectWordUnderCursor(index, charPos, this);
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
    if (!mSelectionImpl->mightStartDrag()) {
        return false;
    }
    const QPoint pos = mouseEvent->pos() - messageRect.topLeft();
    if (mSelectionImpl->textSelection()->hasSelection()) {
        const auto *doc = documentForIndex(index, messageRect.width(), false);
        const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
        if (charPos != -1 && mSelectionImpl->textSelection()->contains(index, charPos)) {
            auto mimeData = new QMimeData;
            mimeData->setHtml(mSelectionImpl->textSelection()->selectedText(TextSelection::Html));
            mimeData->setText(mSelectionImpl->textSelection()->selectedText(TextSelection::Text));
            auto drag = new QDrag(const_cast<QWidget *>(option.widget));
            drag->setMimeData(mimeData);
            drag->exec(Qt::CopyAction);
            mSelectionImpl->setMightStartDrag(false); // don't clear selection on release
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
    Q_UNUSED(msgAttach)
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
