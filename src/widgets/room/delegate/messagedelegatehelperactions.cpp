/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperactions.h"
#include "common/delegatepaintutil.h"
#include "conferencecalldialog/conferenceinfodialog.h"
#include "config-ruqola.h"
#include "misc/avatarcachemanager.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QListView>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QToolTip>

MessageDelegateHelperActions::MessageDelegateHelperActions(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageBlockDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageDelegateHelperActions::~MessageDelegateHelperActions() = default;

void MessageDelegateHelperActions::draw(const Block &block,
                                        QPainter *painter,
                                        QRect blockRect,
                                        const QModelIndex &index,
                                        const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const ActionsLayout layout = layoutActions(block, option, blockRect.width());
    // Draw title and buttons
    const int positionY = blockRect.y() + option.fontMetrics.ascent();
    painter->drawText(blockRect.x(), positionY, layout.title);
}

QSize MessageDelegateHelperActions::sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const ActionsLayout layout = layoutActions(block, option, maxWidth);
    const int height = layout.titleSize.height() + DelegatePaintUtil::margin();
    return {qMax(0, layout.titleSize.width()), height};
}

QPoint MessageDelegateHelperActions::adaptMousePosition(const QPoint &pos, const Block &block, QRect blocksRect, const QStyleOptionViewItem &option)
{
    const ActionsLayout layout = layoutActions(block, option, blocksRect.width());
    const QPoint relativePos = pos - blocksRect.topLeft() - QPoint(0, layout.titleSize.height() + DelegatePaintUtil::margin());
    return relativePos;
}

bool MessageDelegateHelperActions::handleMouseEvent(const Block &block,
                                                    QMouseEvent *mouseEvent,
                                                    QRect blocksRect,
                                                    const QStyleOptionViewItem &option,
                                                    const QModelIndex &index)
{
    Q_UNUSED(index);
    return false;
}

bool MessageDelegateHelperActions::handleHelpEvent(QHelpEvent *helpEvent, QRect blockRect, const Block &block, const QStyleOptionViewItem &option)
{
    return false;
}

MessageDelegateHelperActions::ActionsLayout
MessageDelegateHelperActions::layoutActions(const Block &block, const QStyleOptionViewItem &option, int blockRectWidth) const
{
    Q_UNUSED(blockRectWidth)
    ActionsLayout layout;
    layout.title = block.title();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    return layout;
}

void MessageDelegateHelperActions::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    MessageBlockDelegateHelperBase::setRocketChatAccount(newRocketChatAccount);
}
