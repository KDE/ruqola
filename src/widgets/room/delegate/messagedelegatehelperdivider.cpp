/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperdivider.h"
#include "common/delegatepaintutil.h"

#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>

MessageDelegateHelperDivider::MessageDelegateHelperDivider(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageBlockDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageDelegateHelperDivider::~MessageDelegateHelperDivider() = default;

void MessageDelegateHelperDivider::draw(const Block &block,
                                        QPainter *painter,
                                        QRect blockRect,
                                        const QModelIndex &index,
                                        const QStyleOptionViewItem &option) const
{
    Q_UNUSED(option);
    Q_UNUSED(block);
    Q_UNUSED(index)
    // Draw separator
    // painter->fillRect(blockRect, Qt::red);
    const QPen oldPen = painter->pen();
    QColor col = painter->pen().color();
    col.setAlpha(128);
    painter->setPen(col);
    painter->drawLine(blockRect.x(), blockRect.y(), blockRect.width(), blockRect.y());
    painter->setPen(oldPen);
}

QSize MessageDelegateHelperDivider::sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(option);
    Q_UNUSED(block);
    Q_UNUSED(index)
    const int height = DelegatePaintUtil::margin();
    return {qMax(0, maxWidth), height};
}

bool MessageDelegateHelperDivider::handleMouseEvent(const Block &block,
                                                    QMouseEvent *mouseEvent,
                                                    QRect blocksRect,
                                                    const QStyleOptionViewItem &option,
                                                    const QModelIndex &index)
{
    Q_UNUSED(block);
    Q_UNUSED(mouseEvent);
    Q_UNUSED(blocksRect);
    Q_UNUSED(option);
    Q_UNUSED(index);
    return false;
}
