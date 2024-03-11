/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelpersection.h"
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

MessageDelegateHelperSection::MessageDelegateHelperSection(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageBlockDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageDelegateHelperSection::~MessageDelegateHelperSection() = default;

// Title
void MessageDelegateHelperSection::draw(const Block &block,
                                        QPainter *painter,
                                        QRect blockRect,
                                        const QModelIndex &index,
                                        const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const SectionLayout layout = layoutSection(block, option, blockRect.width());
    // Draw title and buttons
    const int positionY = blockRect.y() + option.fontMetrics.ascent();
    painter->drawText(blockRect.x(), positionY, layout.title);
}

QSize MessageDelegateHelperSection::sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const SectionLayout layout = layoutSection(block, option, maxWidth);
    const int height = layout.titleSize.height() + DelegatePaintUtil::margin();
    return {qMax(0, layout.titleSize.width()), height};
}

QPoint MessageDelegateHelperSection::adaptMousePosition(const QPoint &pos, const Block &block, QRect blocksRect, const QStyleOptionViewItem &option)
{
    const SectionLayout layout = layoutSection(block, option, blocksRect.width());
    const QPoint relativePos = pos - blocksRect.topLeft() - QPoint(0, layout.titleSize.height() + DelegatePaintUtil::margin());
    return relativePos;
}

bool MessageDelegateHelperSection::handleHelpEvent(QHelpEvent *helpEvent, QRect blockRect, const Block &block, const QStyleOptionViewItem &option)
{
    Q_UNUSED(block);
    Q_UNUSED(option);
    Q_UNUSED(helpEvent);
    Q_UNUSED(blockRect);
    // TODO ?
    return false;
}

bool MessageDelegateHelperSection::handleMouseEvent(const Block &block,
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

MessageDelegateHelperSection::SectionLayout
MessageDelegateHelperSection::layoutSection(const Block &block, const QStyleOptionViewItem &option, int blockRectWidth) const
{
    Q_UNUSED(blockRectWidth)
    SectionLayout layout;
    layout.title = block.sectionText();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    return layout;
}

void MessageDelegateHelperSection::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    MessageBlockDelegateHelperBase::setRocketChatAccount(newRocketChatAccount);
}
