/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperbase.h"
#include "messagedelegateutils.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_selection_debug.h"
#include "textconverter.h"

#include <QAbstractTextDocumentLayout>
#include <QClipboard>
#include <QDrag>
#include <QGuiApplication>
#include <QListView>
#include <QMimeData>
#include <QPainter>
#include <QRect>
#include <QStyleOptionViewItem>
#include <QToolTip>

MessageDelegateHelperBase::~MessageDelegateHelperBase() = default;

MessageDelegateHelperBase::MessageDelegateHelperBase(QListView *view, TextSelection *textSelection)
    : mListView(view)
    , mSelection(textSelection)
{
    connect(mSelection, &TextSelection::repaintNeeded, this, &MessageDelegateHelperBase::updateView);
}

void MessageDelegateHelperBase::updateView(const QModelIndex &index)
{
    // qDebug() << " void MessageDelegateHelperBase::updateView(const QModelIndex &index)" << index;
    mListView->update(index);
}

bool MessageDelegateHelperBase::handleMouseEvent(const MessageAttachment &msgAttach,
                                                 QMouseEvent *mouseEvent,
                                                 QRect attachmentsRect,
                                                 const QStyleOptionViewItem &option,
                                                 const QModelIndex &index)
{
    Q_UNUSED(msgAttach)
    Q_UNUSED(option)

    switch (mouseEvent->type()) {
    case QEvent::MouseMove: {
        if (!mMightStartDrag) {
            if (const auto *doc = documentDescriptionForIndex(msgAttach, attachmentsRect.width() /*, true*/)) { // FIXME ME!
                const QPoint pos = mouseEvent->pos();
                const int charPos = charPosition(doc, msgAttach, attachmentsRect, pos, option);
                if (charPos != -1) {
                    // QWidgetTextControl also has code to support isPreediting()/commitPreedit(), selectBlockOnTripleClick
                    mSelection->setEnd(index, charPos, msgAttach);
                    return true;
                }
            }
        }
        break;
    }
    case QEvent::MouseButtonRelease: {
        qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "released";
        setClipboardSelection();
        // Clicks on links
        if (!mSelection->hasSelection()) {
            if (const auto *doc = documentDescriptionForIndex(msgAttach, attachmentsRect.width() /*, true*/)) { // FIXME
                const QPoint pos = mouseEvent->pos();
                const QPoint mouseClickPos = adaptMousePosition(pos, msgAttach, attachmentsRect, option);
                const QString link = doc->documentLayout()->anchorAt(mouseClickPos);
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
    }
    case QEvent::MouseButtonDblClick: {
        if (!mSelection->hasSelection()) {
            if (const auto *doc = documentDescriptionForIndex(msgAttach, attachmentsRect.width() /*, true*/)) { // FIXME ME!
                const QPoint pos = mouseEvent->pos();
                const int charPos = charPosition(doc, msgAttach, attachmentsRect, pos, option);
                qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "double-clicked at pos" << charPos;
                if (charPos == -1) {
                    return false;
                }
                mSelection->selectWordUnderCursor(index, msgAttach, charPos, this);
                return true;
            }
        }
        break;
    }
    case QEvent::MouseButtonPress: {
        mMightStartDrag = false;
        mCurrentIndex = QModelIndex();
        if (const auto *doc = documentDescriptionForIndex(msgAttach, attachmentsRect.width() /*, true*/)) { // FIXME ME!
            const QPoint pos = mouseEvent->pos();
            const int charPos = charPosition(doc, msgAttach, attachmentsRect, pos, option);
            qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "pressed at pos" << charPos;
            if (charPos == -1) {
                return false;
            }
            // TODO fix mSelection->contains with attachment
            if (mSelection->contains(index, charPos) && doc->documentLayout()->hitTest(pos, Qt::ExactHit) != -1) {
                mMightStartDrag = true;
                mCurrentIndex = index;
                return true;
            }

            // QWidgetTextControl also has code to support selectBlockOnTripleClick, shift to extend selection
            // (look there if you want to add these things)

            mSelection->setStart(index, charPos, msgAttach);
            return true;
        } else {
            mSelection->clear();
        }
        break;
    }
    default:
        break;
    }
    return false;
}

bool MessageDelegateHelperBase::maybeStartDrag(const MessageAttachment &msgAttach,
                                               QMouseEvent *mouseEvent,
                                               QRect attachmentsRect,
                                               const QStyleOptionViewItem &option,
                                               const QModelIndex &index)
{
    if (!mMightStartDrag || index != mCurrentIndex || !attachmentsRect.contains(mouseEvent->pos())) {
        return false;
    }

    auto mimeData = new QMimeData;
    mimeData->setUrls({Ruqola::self()->rocketChatAccount()->attachmentUrlFromLocalCache(msgAttach.link())});

    auto drag = new QDrag(const_cast<QWidget *>(option.widget));
    drag->setMimeData(mimeData);
    drag->exec(Qt::CopyAction);

    return true;
}

void MessageDelegateHelperBase::clearTextDocumentCache()
{
    mDocumentCache.clear();
}

int MessageDelegateHelperBase::charPosition(const QTextDocument *doc,
                                            const MessageAttachment &msgAttach,
                                            QRect attachmentsRect,
                                            const QPoint &pos,
                                            const QStyleOptionViewItem &option)
{
    const QPoint mouseClickPos = adaptMousePosition(pos, msgAttach, attachmentsRect, option);
    const int charPos = doc->documentLayout()->hitTest(mouseClickPos, Qt::FuzzyHit);
    return charPos;
}

void MessageDelegateHelperBase::drawDescription(const MessageAttachment &msgAttach,
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

    MessageDelegateUtils::drawSelection(doc, descriptionRect, topPos, painter, index, option, mSelection);
}

QTextDocument *MessageDelegateHelperBase::documentForIndex(const MessageAttachment &msgAttach) const
{
    return documentDescriptionForIndex(msgAttach, -1);
}

QTextDocument *MessageDelegateHelperBase::documentForIndex(const QModelIndex &index) const
{
    Q_UNUSED(index);
    // Unused here
    return nullptr;
}

QSize MessageDelegateHelperBase::documentDescriptionForIndexSize(const MessageAttachment &msgAttach, int width) const
{
    auto *doc = documentDescriptionForIndex(msgAttach, width);
    // Add +10 as if we use only doc->idealWidth() it's too small and it creates a new line.
    return doc ? QSize(doc->idealWidth() + 10, doc->size().height()) : QSize();
}

QTextDocument *MessageDelegateHelperBase::documentDescriptionForIndex(const MessageAttachment &msgAttach, int width) const
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
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    QString needUpdateMessageId; // TODO use it ?
    const QString contextString = TextConverter::convertMessageText(description,
                                                                    rcAccount->userName(),
                                                                    {},
                                                                    rcAccount->highlightWords(),
                                                                    rcAccount->emojiManager(),
                                                                    rcAccount->messageCache(),
                                                                    needUpdateMessageId,
                                                                    {},
                                                                    {});
    auto doc = MessageDelegateUtils::createTextDocument(false, contextString, width);
    auto ret = doc.get();
    mDocumentCache.insert(attachmentId, std::move(doc));
    return ret;
}

void MessageDelegateHelperBase::setClipboardSelection()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    if (mSelection->hasSelection() && clipboard->supportsSelection()) {
        const QString text = mSelection->selectedText(TextSelection::Text, this);
        clipboard->setText(text, QClipboard::Selection);
    }
}

bool MessageDelegateHelperBase::handleHelpEvent(QHelpEvent *helpEvent,
                                                QRect messageRect,
                                                const MessageAttachment &msgAttach,
                                                const QStyleOptionViewItem &option)
{
    Q_UNUSED(option);
    if (helpEvent->type() != QEvent::ToolTip) {
        return false;
    }

    const auto *doc = documentDescriptionForIndex(msgAttach, messageRect.width());
    if (!doc) {
        return false;
    }

    const QPoint pos = adaptMousePosition(helpEvent->pos(), msgAttach, messageRect, option);
    QString formattedTooltip;
    if (MessageDelegateUtils::generateToolTip(doc, pos, formattedTooltip)) {
        QToolTip::showText(helpEvent->globalPos(), formattedTooltip, mListView);
        return true;
    }
    return true;
}
