/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelpertext.h"

#include "colorsandmessageviewstyle.h"
#include "delegateutils/messagedelegateutils.h"
#include "messagecache.h"
#include "model/messagesmodel.h"
#include "model/threadmessagemodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_selection_debug.h"
#include "textconverter.h"
#include "utils.h"

#include <KStringHandler>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QDrag>
#include <QListView>
#include <QMimeData>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QToolTip>

using namespace Qt::Literals::StringLiterals;
MessageDelegateHelperText::MessageDelegateHelperText(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageDelegateHelperText::~MessageDelegateHelperText() = default;

QString MessageDelegateHelperText::makeMessageText(const QPersistentModelIndex &index, bool connectToUpdates) const
{
    const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
    Q_ASSERT(message);
    QString text = index.data(MessagesModel::MessageConvertedText).toString();
    const QByteArray threadMessageId = message->threadMessageId();

    if (mShowThreadContext && !threadMessageId.isEmpty()) {
        const auto sameAsPreviousMessageThread = [&] {
            if (index.row() < 1) {
                return false;
            }
            const auto previousIndex = index.sibling(index.row() - 1, index.column());
            const auto *previousMessage = previousIndex.data(MessagesModel::MessagePointer).value<Message *>();
            Q_ASSERT(previousMessage);
            return threadMessageId == previousMessage->threadMessageId();
        }();
        if (mRocketChatAccount) {
            if (!sameAsPreviousMessageThread) {
                const MessagesModel *model = mRocketChatAccount->messageModelForRoom(message->roomId());
                if (model) {
                    auto that = const_cast<MessageDelegateHelperText *>(this);
                    // Find the previous message in the same thread, to use it as context
                    auto hasSameThread = [&](const Message &msg) {
                        return msg.threadMessageId() == threadMessageId || msg.messageId() == threadMessageId;
                    };
                    Message contextMessage = model->findLastMessageBefore(message->messageId(), hasSameThread);
                    auto messageCache = mRocketChatAccount->messageCache();
                    if (contextMessage.messageId().isEmpty()) {
                        ThreadMessageModel *cachedModel = messageCache->threadMessageModel(threadMessageId);
                        if (cachedModel) {
                            contextMessage = cachedModel->findLastMessageBefore(message->messageId(), hasSameThread);
                            if (contextMessage.messageId().isEmpty()) {
                                Message *msg = messageCache->messageForId(threadMessageId);
                                if (msg) {
                                    contextMessage = *msg;
                                } else if (connectToUpdates) {
                                    connect(messageCache, &MessageCache::messageLoaded, this, [threadMessageId, that, index](const QByteArray &msgId) {
                                        if (msgId == threadMessageId) {
                                            that->updateView(index);
                                        }
                                    });
                                }
                            } else {
                                // qDebug() << "using cache, found" << contextMessage.messageId() << contextMessage.text();
                            }
                        } else if (connectToUpdates) {
                            connect(messageCache, &MessageCache::modelLoaded, this, [that, index]() {
                                that->updateView(index);
                            });
                        }
                    }
                    // Use TextConverter in case it starts with a [](URL) reply marker
                    const QString contextText = KStringHandler::rsqueeze(u'@' + contextMessage.username() + ": "_L1 + contextMessage.text(), 200);

                    QByteArray needUpdateMessageId;
                    const int maximumRecursiveQuotedText = mRocketChatAccount->ruqolaServerConfig()->messageQuoteChainLimit();
                    const TextConverter::ConvertMessageTextSettings settings(contextText,
                                                                             mRocketChatAccount->userName(),
                                                                             {},
                                                                             mRocketChatAccount->highlightWords(),
                                                                             mRocketChatAccount->emojiManager(),
                                                                             mRocketChatAccount->messageCache(),
                                                                             contextMessage.mentions(),
                                                                             contextMessage.channels(),
                                                                             {},
                                                                             mSearchText,
                                                                             maximumRecursiveQuotedText);

                    int recursiveIndex = 0;
                    int numberOfTextSearched = 0;
                    const int hightLightStringIndex = 0;
                    const QString contextString =
                        TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex, numberOfTextSearched, hightLightStringIndex);
                    if (!needUpdateMessageId.isEmpty() && connectToUpdates) {
                        connect(messageCache, &MessageCache::messageLoaded, this, [needUpdateMessageId, that, index](const QByteArray &msgId) {
                            if (msgId == needUpdateMessageId) {
                                that->updateView(index);
                            }
                        });
                    }
                    // TODO add url ?
                    Utils::QuotedRichTextInfo info;
                    info.richText = contextString;
                    text.prepend(Utils::formatQuotedRichText(std::move(info)));
                }
            }
        }
    }

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

void MessageDelegateHelperText::draw(QPainter *painter, QRect rect, const QModelIndex &index, const QStyleOptionViewItem &option)
{
    auto *doc = documentForIndex(index, rect.width(), true);
    if (!doc) {
        return;
    }
    MessageDelegateUtils::drawSelection(doc, rect, rect.top(), painter, index, option, mTextSelectionImpl->textSelection(), {}, {});
}

QSize MessageDelegateHelperText::sizeHint(const QModelIndex &index, int maxWidth, [[maybe_unused]] const QStyleOptionViewItem &option, qreal *pBaseLine) const
{
    auto *doc = documentForIndex(index, maxWidth, true);
    return MessageDelegateUtils::textSizeHint(doc, pBaseLine);
}

bool MessageDelegateHelperText::handleMouseEvent(QMouseEvent *mouseEvent,
                                                 QRect messageRect,
                                                 [[maybe_unused]] const QStyleOptionViewItem &option,
                                                 const QModelIndex &index)
{
    if (!messageRect.contains(mouseEvent->pos())) {
        return false;
    }

    const QPoint pos = mouseEvent->pos() - messageRect.topLeft();
    const QEvent::Type eventType = mouseEvent->type();
    // Text selection
    switch (eventType) {
    case QEvent::MouseButtonPress:
        mTextSelectionImpl->setMightStartDrag(false);
        if (const auto *doc = documentForIndex(index, messageRect.width(), true)) {
            const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
            qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "pressed at pos" << charPos;
            if (charPos == -1) {
                return false;
            }
            if (mTextSelectionImpl->textSelection()->contains(index, charPos) && doc->documentLayout()->hitTest(pos, Qt::ExactHit) != -1) {
                mTextSelectionImpl->setMightStartDrag(true);
                return true;
            }

            // QWidgetTextControl also has code to support selectBlockOnTripleClick, shift to extend selection
            // (look there if you want to add these things)

            mTextSelectionImpl->textSelection()->setTextSelectionStart(index, charPos);
            return true;
        } else {
            mTextSelectionImpl->textSelection()->clear();
        }
        break;
    case QEvent::MouseMove:
        if (!mTextSelectionImpl->mightStartDrag()) {
            if (const auto *doc = documentForIndex(index, messageRect.width(), true)) {
                const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
                if (charPos != -1) {
                    // QWidgetTextControl also has code to support isPreediting()/commitPreedit(), selectBlockOnTripleClick
                    mTextSelectionImpl->textSelection()->setTextSelectionEnd(index, charPos);
                    return true;
                }
            }
        }
        break;
    case QEvent::MouseButtonRelease:
        qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "released";
        MessageDelegateUtils::setClipboardSelection(mTextSelectionImpl->textSelection());
        // Clicks on links
        if (!mTextSelectionImpl->textSelection()->hasSelection()) {
            if (const auto *doc = documentForIndex(index, messageRect.width(), true)) {
                const QString link = doc->documentLayout()->anchorAt(pos);
                if (!link.isEmpty()) {
                    Q_EMIT mRocketChatAccount->openLinkRequested(link);
                    return true;
                }
            }
        } else if (mTextSelectionImpl->mightStartDrag()) {
            // clicked into selection, didn't start drag, clear it (like kwrite and QTextEdit)
            mTextSelectionImpl->textSelection()->clear();
        }
        // don't return true here, we need to send mouse release events to other helpers (ex: click on image)
        break;
    case QEvent::MouseButtonDblClick:
        if (!mTextSelectionImpl->textSelection()->hasSelection()) {
            if (const auto *doc = documentForIndex(index, messageRect.width(), true)) {
                const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
                qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "double-clicked at pos" << charPos;
                if (charPos == -1) {
                    return false;
                }
                mTextSelectionImpl->textSelection()->selectWordUnderCursor(index, charPos, this);
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
    if (!mTextSelectionImpl->mightStartDrag()) {
        return false;
    }
    if (mTextSelectionImpl->textSelection()->hasSelection()) {
        const QPoint pos = mouseEvent->pos() - messageRect.topLeft();
        const auto *doc = documentForIndex(index, messageRect.width(), false);
        const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
        if (charPos != -1 && mTextSelectionImpl->textSelection()->contains(index, charPos)) {
            auto mimeData = new QMimeData;
            mimeData->setHtml(mTextSelectionImpl->textSelection()->selectedText(TextSelection::Format::Html));
            mimeData->setText(mTextSelectionImpl->textSelection()->selectedText(TextSelection::Format::Text));
            auto drag = new QDrag(const_cast<QWidget *>(option.widget));
            drag->setMimeData(mimeData);
            drag->exec(Qt::CopyAction);
            mTextSelectionImpl->setMightStartDrag(false); // don't clear selection on release
            return true;
        }
    }
    return false;
}

void MessageDelegateHelperText::setShowThreadContext(bool b)
{
    mShowThreadContext = b;
}

bool MessageDelegateHelperText::showThreadContext() const
{
    return mShowThreadContext;
}

QTextDocument *MessageDelegateHelperText::documentForIndex(const QModelIndex &index) const
{
    return documentForIndex(index, -1, false);
}

QTextDocument *MessageDelegateHelperText::documentForIndex(const QModelIndex &index, int width, bool connectToUpdates) const
{
    Q_ASSERT(index.isValid());
    const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
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

    const auto persistentIndex = QPersistentModelIndex(index);
    const QString text = makeMessageText(persistentIndex, connectToUpdates);
    if (text.isEmpty()) {
        return nullptr;
    }
    auto doc = MessageDelegateUtils::createTextDocument(MessageDelegateUtils::useItalicsForMessage(index), text, width);
    auto ret = doc.get();
    connect(&ColorsAndMessageViewStyle::self(), &ColorsAndMessageViewStyle::needToUpdateColors, ret, [this, persistentIndex, ret]() {
        ret->setHtml(makeMessageText(persistentIndex, false));
        auto that = const_cast<MessageDelegateHelperText *>(this);
        that->updateView(persistentIndex);
    });
    mDocumentCache.insert(messageId, std::move(doc));
    return ret;
}

#include "moc_messagedelegatehelpertext.cpp"
