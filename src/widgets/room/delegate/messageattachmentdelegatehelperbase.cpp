/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelperbase.h"
#include "delegateutils/messagedelegateutils.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_selection_debug.h"

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
    connect(mTextSelectionImpl->textSelection(), &TextSelection::repaintNeeded, this, &MessageAttachmentDelegateHelperBase::updateView);
}

bool MessageAttachmentDelegateHelperBase::handleMouseEvent(const MessageAttachment &msgAttach,
                                                           QMouseEvent *mouseEvent,
                                                           QRect attachmentsRect,
                                                           const QStyleOptionViewItem &option,
                                                           const QModelIndex &index)
{
    switch (mouseEvent->type()) {
    case QEvent::MouseMove: {
        if (!mTextSelectionImpl->mightStartDrag()) {
            if (const auto *doc =
                    documentDescriptionForIndex(convertAttachmentToDocumentDescriptionInfo(msgAttach, attachmentsRect.width()) /*, true*/)) { // FIXME ME!
                const QPoint pos = mouseEvent->pos();
                const int charPos = charPosition(doc, msgAttach, attachmentsRect, pos, option);
                if (charPos != -1) {
                    // QWidgetTextControl also has code to support isPreediting()/commitPreedit(), selectBlockOnTripleClick
                    mTextSelectionImpl->textSelection()->setEnd(index, charPos, msgAttach);
                    return true;
                }
            }
        }
        break;
    }
    case QEvent::MouseButtonRelease: {
        qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "released";
        MessageDelegateUtils::setClipboardSelection(mTextSelectionImpl->textSelection());
        // Clicks on links
        if (!mTextSelectionImpl->textSelection()->hasSelection()) {
            if (const auto *doc =
                    documentDescriptionForIndex(convertAttachmentToDocumentDescriptionInfo(msgAttach, attachmentsRect.width()) /*, true*/)) { // FIXME
                const QPoint pos = mouseEvent->pos();
                const QPoint mouseClickPos = adaptMousePosition(pos, msgAttach, attachmentsRect, option);
                const QString link = doc->documentLayout()->anchorAt(mouseClickPos);
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
    }
    case QEvent::MouseButtonDblClick: {
        if (!mTextSelectionImpl->textSelection()->hasSelection()) {
            if (const auto *doc =
                    documentDescriptionForIndex(convertAttachmentToDocumentDescriptionInfo(msgAttach, attachmentsRect.width()) /*, true*/)) { // FIXME ME!
                const QPoint pos = mouseEvent->pos();
                const int charPos = charPosition(doc, msgAttach, attachmentsRect, pos, option);
                qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "double-clicked at pos" << charPos;
                if (charPos == -1) {
                    return false;
                }
                mTextSelectionImpl->textSelection()->selectWordUnderCursor(index, charPos, this, msgAttach);
                return true;
            }
        }
        break;
    }
    case QEvent::MouseButtonPress: {
        mTextSelectionImpl->setMightStartDrag(false);
        mCurrentIndex = QModelIndex();
        if (const auto *doc =
                documentDescriptionForIndex(convertAttachmentToDocumentDescriptionInfo(msgAttach, attachmentsRect.width()) /*, true*/)) { // FIXME ME!
            const QPoint pos = mouseEvent->pos();
            const int charPos = charPosition(doc, msgAttach, attachmentsRect, pos, option);
            qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "pressed at pos" << charPos;
            if (charPos == -1) {
                return false;
            }
            // TODO fix mTextSelectionImpl->contains with attachment
            if (mTextSelectionImpl->textSelection()->contains(index, charPos) && doc->documentLayout()->hitTest(pos, Qt::ExactHit) != -1) {
                mTextSelectionImpl->setMightStartDrag(true);
                mCurrentIndex = index;
                return true;
            }

            // QWidgetTextControl also has code to support selectBlockOnTripleClick, shift to extend selection
            // (look there if you want to add these things)

            mTextSelectionImpl->textSelection()->setStart(index, charPos, msgAttach);
            return true;
        } else {
            mTextSelectionImpl->textSelection()->clear();
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
    if (!mTextSelectionImpl->mightStartDrag() || index != mCurrentIndex || !attachmentsRect.contains(mouseEvent->pos())) {
        return false;
    }

    auto mimeData = new QMimeData;
    mimeData->setUrls({mRocketChatAccount->attachmentUrlFromLocalCache(msgAttach.link())});

    auto drag = new QDrag(const_cast<QWidget *>(option.widget));
    drag->setMimeData(mimeData);
    drag->exec(Qt::CopyAction);

    return true;
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
    auto *doc = documentDescriptionForIndex(convertAttachmentToDocumentDescriptionInfo(msgAttach, descriptionRect.width()));
    if (!doc) {
        return;
    }

    MessageDelegateUtils::drawSelection(doc, descriptionRect, topPos, painter, index, option, mTextSelectionImpl->textSelection(), msgAttach, {});
}

QTextDocument *MessageAttachmentDelegateHelperBase::documentForAttachement(const MessageAttachment &msgAttach) const
{
    return documentDescriptionForIndex(convertAttachmentToDocumentDescriptionInfo(msgAttach, -1));
}

MessageDelegateHelperBase::DocumentDescriptionInfo
MessageAttachmentDelegateHelperBase::convertAttachmentToDocumentDescriptionInfo(const MessageAttachment &msgAttach, int width) const
{
    MessageDelegateHelperBase::DocumentDescriptionInfo info;
    info.documentId = msgAttach.attachmentId();
    info.description = msgAttach.description();
    info.width = width;
    return info;
}

bool MessageAttachmentDelegateHelperBase::handleHelpEvent(QHelpEvent *helpEvent,
                                                          QRect messageRect,
                                                          const MessageAttachment &msgAttach,
                                                          const QStyleOptionViewItem &option)
{
    if (helpEvent->type() != QEvent::ToolTip) {
        return false;
    }

    const auto *doc = documentDescriptionForIndex(convertAttachmentToDocumentDescriptionInfo(msgAttach, messageRect.width()));
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
    auto document = documentDescriptionForIndex(convertAttachmentToDocumentDescriptionInfo(msgAttach, attachmentsRect.width()) /*, true*/);
    if (!document) {
        return {};
    }
    const QPoint relativePos = adaptMousePosition(pos, msgAttach, attachmentsRect, option);
    return document->documentLayout()->anchorAt(relativePos);
}

bool MessageAttachmentDelegateHelperBase::contextMenu(const QPoint &pos,
                                                      const QPoint &globalPos,
                                                      const MessageAttachment &msgAttach,
                                                      QRect attachmentsRect,
                                                      const QStyleOptionViewItem &option,
                                                      QMenu *menu)
{
    Q_UNUSED(msgAttach);
    Q_UNUSED(attachmentsRect);
    Q_UNUSED(pos);
    Q_UNUSED(option);
    Q_UNUSED(globalPos);
    Q_UNUSED(menu);
    return false;
}
