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

void MessageDelegateHelperDivider::draw([[maybe_unused]] const Block &block,
                                        QPainter *painter,
                                        QRect blockRect,
                                        [[maybe_unused]] const QModelIndex &index,
                                        [[maybe_unused]] const QStyleOptionViewItem &option) const
{
    // Draw separator
    // painter->fillRect(blockRect, Qt::red);
    const QPen oldPen = painter->pen();
    QColor col = painter->pen().color();
    col.setAlpha(128);
    painter->setPen(col);
    painter->drawLine(blockRect.x(), blockRect.y(), blockRect.width(), blockRect.y());
    painter->setPen(oldPen);
}

QSize MessageDelegateHelperDivider::sizeHint([[maybe_unused]] const Block &block,
                                             [[maybe_unused]] const QModelIndex &index,
                                             int maxWidth,
                                             [[maybe_unused]] const QStyleOptionViewItem &option) const
{
    const int height = DelegatePaintUtil::margin();
    return {qMax(0, maxWidth), height};
}

bool MessageDelegateHelperDivider::handleMouseEvent([[maybe_unused]] const Block &block,
                                                    [[maybe_unused]] QMouseEvent *mouseEvent,
                                                    [[maybe_unused]] QRect blocksRect,
                                                    [[maybe_unused]] const QStyleOptionViewItem &option,
                                                    [[maybe_unused]] const QModelIndex &index)
{
    return false;
}
