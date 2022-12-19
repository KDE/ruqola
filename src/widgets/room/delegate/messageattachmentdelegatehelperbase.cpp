/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelperbase.h"
#include "delegateutils/messagedelegateutils.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_selection_debug.h"
#include "textconverter.h"

#include <QAbstractTextDocumentLayout>
#include <QDrag>
#include <QListView>
#include <QMimeData>
#include <QPainter>
#include <QRect>
#include <QStyleOptionViewItem>
#include <QToolTip>

MessageAttachmentDelegateHelperBase::~MessageAttachmentDelegateHelperBase() = default;

MessageAttachmentDelegateHelperBase::MessageAttachmentDelegateHelperBase(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageDelegateHelperBase(account, view, textSelectionImpl)
{
    connect(mSelectionImpl->textSelection(), &TextSelection::repaintNeeded, this, &MessageAttachmentDelegateHelperBase::updateView);
}

bool MessageAttachmentDelegateHelperBase::handleMouseEvent(const MessageAttachment &msgAttach,
                                                           QMouseEvent *mouseEvent,
                                                           QRect attachmentsRect,
                                                           const QStyleOptionViewItem &option,
                                                           const QModelIndex &index)
{
    switch (mouseEvent->type()) {
    case QEvent::MouseMove: {
        if (!mSelectionImpl->mightStartDrag()) {
            if (const auto *doc = documentDescriptionForIndex(msgAttach, attachmentsRect.width() /*, true*/)) { // FIXME ME!
                const QPoint pos = mouseEvent->pos();
                const int charPos = charPosition(doc, msgAttach, attachmentsRect, pos, option);
                if (charPos != -1) {
                    // QWidgetTextControl also has code to support isPreediting()/commitPreedit(), selectBlockOnTripleClick
                    mSelectionImpl->textSelection()->setEnd(index, charPos, msgAttach);
                    return true;
                }
            }
        }
        break;
    }
    case QEvent::MouseButtonRelease: {
        qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "released";
        MessageDelegateUtils::setClipboardSelection(mSelectionImpl->textSelection());
        // Clicks on links
        if (!mSelectionImpl->textSelection()->hasSelection()) {
            if (const auto *doc = documentDescriptionForIndex(msgAttach, attachmentsRect.width() /*, true*/)) { // FIXME
                const QPoint pos = mouseEvent->pos();
                const QPoint mouseClickPos = adaptMousePosition(pos, msgAttach, attachmentsRect, option);
                const QString link = doc->documentLayout()->anchorAt(mouseClickPos);
                if (!link.isEmpty()) {
                    Q_EMIT mRocketChatAccount->openLinkRequested(link);
                    return true;
                }
            }
        } else if (mSelectionImpl->mightStartDrag()) {
            // clicked into selection, didn't start drag, clear it (like kwrite and QTextEdit)
            mSelectionImpl->textSelection()->clear();
        }
        // don't return true here, we need to send mouse release events to other helpers (ex: click on image)
        break;
    }
    case QEvent::MouseButtonDblClick: {
        if (!mSelectionImpl->textSelection()->hasSelection()) {
            if (const auto *doc = documentDescriptionForIndex(msgAttach, attachmentsRect.width() /*, true*/)) { // FIXME ME!
                const QPoint pos = mouseEvent->pos();
                const int charPos = charPosition(doc, msgAttach, attachmentsRect, pos, option);
                qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "double-clicked at pos" << charPos;
                if (charPos == -1) {
                    return false;
                }
                mSelectionImpl->textSelection()->selectWordUnderCursor(index, charPos, this, msgAttach);
                return true;
            }
        }
        break;
    }
    case QEvent::MouseButtonPress: {
        mSelectionImpl->setMightStartDrag(false);
        mCurrentIndex = QModelIndex();
        if (const auto *doc = documentDescriptionForIndex(msgAttach, attachmentsRect.width() /*, true*/)) { // FIXME ME!
            const QPoint pos = mouseEvent->pos();
            const int charPos = charPosition(doc, msgAttach, attachmentsRect, pos, option);
            qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "pressed at pos" << charPos;
            if (charPos == -1) {
                return false;
            }
            // TODO fix mSelectionImpl->contains with attachment
            if (mSelectionImpl->textSelection()->contains(index, charPos) && doc->documentLayout()->hitTest(pos, Qt::ExactHit) != -1) {
                mSelectionImpl->setMightStartDrag(true);
                mCurrentIndex = index;
                return true;
            }

            // QWidgetTextControl also has code to support selectBlockOnTripleClick, shift to extend selection
            // (look there if you want to add these things)

            mSelectionImpl->textSelection()->setStart(index, charPos, msgAttach);
            return true;
        } else {
            mSelectionImpl->textSelection()->clear();
        }
        break;
    }
    default:
        break;
    }
    return false;
}

bool MessageAttachmentDelegateHelperBase::maybeStartDrag(const MessageAttachment &msgAttach,
                                                         QMouseEvent *mouseEvent,
                                                         QRect attachmentsRect,
                                                         const QStyleOptionViewItem &option,
                                                         const QModelIndex &index)
{
    if (!mSelectionImpl->mightStartDrag() || index != mCurrentIndex || !attachmentsRect.contains(mouseEvent->pos())) {
        return false;
    }

    auto mimeData = new QMimeData;
    mimeData->setUrls({mRocketChatAccount->attachmentUrlFromLocalCache(msgAttach.link())});

    auto drag = new QDrag(const_cast<QWidget *>(option.widget));
    drag->setMimeData(mimeData);
    drag->exec(Qt::CopyAction);

    return true;
}

void MessageAttachmentDelegateHelperBase::clearTextDocumentCache()
{
    mDocumentCache.clear();
}

int MessageAttachmentDelegateHelperBase::charPosition(const QTextDocument *doc,
                                                      const MessageAttachment &msgAttach,
                                                      QRect attachmentsRect,
                                                      const QPoint &pos,
                                                      const QStyleOptionViewItem &option)
{
    const QPoint relativePos = adaptMousePosition(pos, msgAttach, attachmentsRect, option);
    const int charPos = doc->documentLayout()->hitTest(relativePos, Qt::FuzzyHit);
    return charPos;
}

void MessageAttachmentDelegateHelperBase::drawDescription(const MessageAttachment &msgAttach,
                                                          QRect descriptionRect,
                                                          QPainter *painter,
                                                          int topPos,
                                                          const QModelIndex &index,
                                                          const QStyleOptionViewItem &option) const
{
    auto *doc = documentDescriptionForIndex(msgAttach, descriptionRect.width());
    if (!doc) {
        return;
    }

    MessageDelegateUtils::drawSelection(doc, descriptionRect, topPos, painter, index, option, mSelectionImpl->textSelection(), msgAttach);
}

void MessageAttachmentDelegateHelperBase::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mRocketChatAccount = newRocketChatAccount;
}

QTextDocument *MessageAttachmentDelegateHelperBase::documentForIndex(const MessageAttachment &msgAttach) const
{
    return documentDescriptionForIndex(msgAttach, -1);
}

QTextDocument *MessageAttachmentDelegateHelperBase::documentForIndex(const QModelIndex &index) const
{
    Q_UNUSED(index)
    // Unused here
    return nullptr;
}

QSize MessageAttachmentDelegateHelperBase::documentDescriptionForIndexSize(const MessageAttachment &msgAttach, int width) const
{
    auto *doc = documentDescriptionForIndex(msgAttach, width);
    // Add +10 as if we use only doc->idealWidth() it's too small and it creates a new line.
    return doc ? QSize(doc->idealWidth() + 10, doc->size().height()) : QSize();
}

QTextDocument *MessageAttachmentDelegateHelperBase::documentDescriptionForIndex(const MessageAttachment &msgAttach, int width) const
{
    const QString attachmentId = msgAttach.attachmentId();
    auto it = mDocumentCache.find(attachmentId);
    if (it != mDocumentCache.end()) {
        auto ret = it->value.get();
        if (width != -1 && !qFuzzyCompare(ret->textWidth(), width)) {
            ret->setTextWidth(width);
        }
        return ret;
    }

    const QString description = msgAttach.description();

    if (description.isEmpty()) {
        return nullptr;
    }
    // Use TextConverter in case it starts with a [](URL) reply marker
    QString needUpdateMessageId; // TODO use it ?
    // Laurent Ruqola::self()->rocketChatAccount() only for test.
    auto account = mRocketChatAccount ? mRocketChatAccount : Ruqola::self()->rocketChatAccount();
    const QString contextString = TextConverter::convertMessageText(description,
                                                                    account->userName(),
                                                                    {},
                                                                    account->highlightWords(),
                                                                    account->emojiManager(),
                                                                    account->messageCache(),
                                                                    needUpdateMessageId,
                                                                    {},
                                                                    {});
    auto doc = MessageDelegateUtils::createTextDocument(false, contextString, width);
    auto ret = doc.get();
    mDocumentCache.insert(attachmentId, std::move(doc));
    return ret;
}

bool MessageAttachmentDelegateHelperBase::handleHelpEvent(QHelpEvent *helpEvent,
                                                          QRect messageRect,
                                                          const MessageAttachment &msgAttach,
                                                          const QStyleOptionViewItem &option)
{
    if (helpEvent->type() != QEvent::ToolTip) {
        return false;
    }

    const auto *doc = documentDescriptionForIndex(msgAttach, messageRect.width());
    if (!doc) {
        return false;
    }

    const QPoint relativePos = adaptMousePosition(helpEvent->pos(), msgAttach, messageRect, option);
    QString formattedTooltip;
    if (MessageDelegateUtils::generateToolTip(doc, relativePos, formattedTooltip)) {
        QToolTip::showText(helpEvent->globalPos(), formattedTooltip, mListView);
        return true;
    }
    return true;
}

QString MessageAttachmentDelegateHelperBase::urlAt(const QStyleOptionViewItem &option, const MessageAttachment &msgAttach, QRect attachmentsRect, QPoint pos)
{
    auto document = documentDescriptionForIndex(msgAttach, attachmentsRect.width() /*, true*/);
    if (!document) {
        return {};
    }
    const QPoint relativePos = adaptMousePosition(pos, msgAttach, attachmentsRect, option);
    return document->documentLayout()->anchorAt(relativePos);
}
