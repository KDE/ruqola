/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageblockdelegatehelperbase.h"

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
