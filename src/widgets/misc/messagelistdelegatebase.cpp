/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistdelegatebase.h"
#include "delegateutils/messagedelegateutils.h"
#include "delegateutils/textselectionimpl.h"

#include "rocketchataccount.h"
#include "ruqolawidgets_selection_debug.h"

#include <QAbstractTextDocumentLayout>
#include <QDrag>
#include <QListView>
#include <QMimeData>
#include <QMouseEvent>

MessageListDelegateBase::MessageListDelegateBase(QListView *view, QObject *parent)
    : QItemDelegate{parent}
    , mTextSelectionImpl(new TextSelectionImpl)
    , mListView(view)
{
    auto textSelection = mTextSelectionImpl->textSelection();
    textSelection->setTextHelperFactory(this);
    connect(textSelection, &TextSelection::repaintNeeded, this, &MessageListDelegateBase::updateView);
}

MessageListDelegateBase::~MessageListDelegateBase()
{
    delete mTextSelectionImpl;
}

void MessageListDelegateBase::clearCache()
{
    mDocumentCache.clear();
}

bool MessageListDelegateBase::maybeStartDrag(QMouseEvent *mouseEvent, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (!mTextSelectionImpl->mightStartDrag()) {
        return false;
    }
    const QPoint pos = mouseEvent->pos() - messageRect.topLeft();
    if (mTextSelectionImpl->textSelection()->hasSelection()) {
        const auto *doc = documentForModelIndex(index, messageRect.width());
        const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
        if (charPos != -1 && mTextSelectionImpl->textSelection()->contains(index, charPos)) {
            auto mimeData = new QMimeData;
            mimeData->setHtml(mTextSelectionImpl->textSelection()->selectedText(TextSelection::Html));
            mimeData->setText(mTextSelectionImpl->textSelection()->selectedText(TextSelection::Text));
            auto drag = new QDrag(const_cast<QWidget *>(option.widget));
            drag->setMimeData(mimeData);
            drag->exec(Qt::CopyAction);
            mTextSelectionImpl->setMightStartDrag(false); // don't clear selection on release
            return true;
        }
    }
    return false;
}

bool MessageListDelegateBase::handleMouseEvent(QMouseEvent *mouseEvent, QRect messageRect, const QStyleOptionViewItem &option, const QModelIndex &index)
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
        mTextSelectionImpl->setMightStartDrag(false);
        if (const auto *doc = documentForModelIndex(index, messageRect.width())) {
            const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
            qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "pressed at pos" << charPos;
            if (charPos == -1) {
                return false;
            }
            if (mTextSelectionImpl->textSelection()->contains(index, charPos) && doc->documentLayout()->hitTest(pos, Qt::ExactHit) != -1) {
                mTextSelectionImpl->setMightStartDrag(true);
                return true;
            }

            // TODO add triple click

            // QWidgetTextControl also has code to support selectBlockOnTripleClick, shift to extend selection
            // (look there if you want to add these things)

            mTextSelectionImpl->textSelection()->setStart(index, charPos);
            return true;
        } else {
            mTextSelectionImpl->textSelection()->clear();
        }
        break;
    case QEvent::MouseMove:
        if (!mTextSelectionImpl->mightStartDrag()) {
            if (const auto *doc = documentForModelIndex(index, messageRect.width())) {
                const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
                if (charPos != -1) {
                    // QWidgetTextControl also has code to support isPreediting()/commitPreedit(), selectBlockOnTripleClick
                    mTextSelectionImpl->textSelection()->setEnd(index, charPos);
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
            if (const auto *doc = documentForModelIndex(index, messageRect.width())) {
                const QString link = doc->documentLayout()->anchorAt(pos);
                if (!link.isEmpty()) {
                    Q_EMIT rocketChatAccount(index)->openLinkRequested(link);
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
            if (const auto *doc = documentForModelIndex(index, messageRect.width())) {
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

QTextDocument *MessageListDelegateBase::documentForIndex(const QModelIndex &index) const
{
    return documentForModelIndex(index, -1);
}

QTextDocument *MessageListDelegateBase::documentForIndex(const MessageAttachment &msgAttach) const
{
    Q_UNUSED(msgAttach)
    Q_ASSERT(false);
    // Unused here
    return nullptr;
}

QTextDocument *MessageListDelegateBase::documentForIndex(const Block &block) const
{
    Q_UNUSED(block)
    Q_ASSERT(false);
    // Unused here
    return nullptr;
}

QSize MessageListDelegateBase::textSizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option, qreal *pBaseLine) const
{
    Q_UNUSED(option)
    auto *doc = documentForModelIndex(index, maxWidth);
    return MessageDelegateUtils::textSizeHint(doc, pBaseLine);
}

void MessageListDelegateBase::selectAll(const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(option);
    mTextSelectionImpl->textSelection()->selectMessage(index);
    mListView->update(index);
    MessageDelegateUtils::setClipboardSelection(mTextSelectionImpl->textSelection());
}

QString MessageListDelegateBase::selectedText() const
{
    return mTextSelectionImpl->textSelection()->selectedText(TextSelection::Format::Text);
}

bool MessageListDelegateBase::hasSelection() const
{
    return mTextSelectionImpl->textSelection()->hasSelection();
}

const QString &MessageListDelegateBase::searchText() const
{
    return mSearchText;
}

void MessageListDelegateBase::setSearchText(const QString &newSearchText)
{
    if (mSearchText != newSearchText) {
        mSearchText = newSearchText;
        clearCache();
    }
}
