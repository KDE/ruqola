/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageblockdelegatehelperbase.h"
#include "delegateutils/messagedelegateutils.h"

#include <QAbstractItemView>
#include <QDrag>
#include <QHelpEvent>
#include <QMimeData>

MessageBlockDelegateHelperBase::MessageBlockDelegateHelperBase(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageBlockDelegateHelperBase::~MessageBlockDelegateHelperBase() = default;

bool MessageBlockDelegateHelperBase::handleHelpEvent(QHelpEvent *helpEvent, QRect messageRect, const Block &block, const QStyleOptionViewItem &option)
{
    if (helpEvent->type() != QEvent::ToolTip) {
        return false;
    }
#if 0
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
#endif
    return true;
}

bool MessageBlockDelegateHelperBase::maybeStartDrag(const Block &block,
                                                    QMouseEvent *event,
                                                    QRect attachmentsRect,
                                                    const QStyleOptionViewItem &option,
                                                    const QModelIndex &index)
{
#if 0
    if (!mSelectionImpl->mightStartDrag() || index != mCurrentIndex || !attachmentsRect.contains(mouseEvent->pos())) {
        return false;
    }

    auto mimeData = new QMimeData;
    mimeData->setUrls({mRocketChatAccount->attachmentUrlFromLocalCache(msgAttach.link())});

    auto drag = new QDrag(const_cast<QWidget *>(option.widget));
    drag->setMimeData(mimeData);
    drag->exec(Qt::CopyAction);

    return true;
#else
    return false;
#endif
}

QTextDocument *MessageBlockDelegateHelperBase::documentDescriptionForIndex(const Block &block, int width) const
{
    const QString attachmentId = block.blockId();
    auto it = mDocumentCache.find(attachmentId);
    if (it != mDocumentCache.end()) {
        auto ret = it->value.get();
        if (width != -1 && !qFuzzyCompare(ret->textWidth(), width)) {
            ret->setTextWidth(width);
        }
        return ret;
    }
#if 0
    const QString description = block.description();

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
#else
    return nullptr;
#endif
}

QSize MessageBlockDelegateHelperBase::documentDescriptionForIndexSize(const Block &block, int width) const
{
    auto *doc = documentDescriptionForIndex(block, width);
    // Add +10 as if we use only doc->idealWidth() it's too small and it creates a new line.
    return doc ? QSize(doc->idealWidth() + 10, doc->size().height()) : QSize();
}

void MessageBlockDelegateHelperBase::drawDescription(const Block &block,
                                                     QRect descriptionRect,
                                                     QPainter *painter,
                                                     int topPos,
                                                     const QModelIndex &index,
                                                     const QStyleOptionViewItem &option) const
{
    auto *doc = documentDescriptionForIndex(block, descriptionRect.width());
    if (!doc) {
        return;
    }

    // TODO MessageDelegateUtils::drawSelection(doc, descriptionRect, topPos, painter, index, option, mSelectionImpl->textSelection(), msgAttach);
}
