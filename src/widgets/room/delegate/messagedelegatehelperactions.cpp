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
    for (const auto &button : layout.buttonList) {
        // Draw join button
        const QPen origPen = painter->pen();
        const QBrush origBrush = painter->brush();
        const QPen buttonPen(option.palette.color(QPalette::Highlight).darker());
        QColor backgroundColor = option.palette.color(QPalette::Highlight);
        backgroundColor.setAlpha(60);
        const QBrush buttonBrush(backgroundColor);
        const QRectF joinButtonRect = button.buttonRect.translated(blockRect.topLeft());
        // Rounded rect
        painter->setPen(buttonPen);
        painter->setBrush(buttonBrush);
        painter->drawRoundedRect(joinButtonRect, 5, 5);
        painter->setBrush(origBrush);
        painter->setPen(origPen);
        const QRectF r = joinButtonRect.adjusted((joinButtonRect.width() - button.buttonRect.width()) / 2, 0, 0, 0);
        // FIXME: Draw center !
        painter->drawText(r, button.text);
    }
}

QSize MessageDelegateHelperActions::sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const ActionsLayout layout = layoutActions(block, option, maxWidth);
    if (layout.buttonList.isEmpty()) {
        return {};
    }
    const int height = layout.buttonList.at(0).buttonRect.height() + DelegatePaintUtil::margin();
    // TODO fix width
    return {qMax(0, static_cast<int>(layout.buttonList.at(0).buttonRect.width())), height};
}

QPoint MessageDelegateHelperActions::adaptMousePosition(const QPoint &pos, const Block &block, QRect blocksRect, const QStyleOptionViewItem &option)
{
    const ActionsLayout layout = layoutActions(block, option, blocksRect.width());
    const QPoint relativePos; // TODO = pos - blocksRect.topLeft() - QPoint(0, layout.titleSize.height() + DelegatePaintUtil::margin());
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

    qreal x = 0;
    const QList<BlockAction> actions = block.blockActions();
    for (const auto &act : actions) {
        ButtonLayout buttonLayout;
        buttonLayout.text = act.text();
        const QSize buttonSize = option.fontMetrics.size(Qt::TextSingleLine, buttonLayout.text);
        buttonLayout.buttonRect = QRectF(x, DelegatePaintUtil::margin(), buttonSize.width() + 2 * DelegatePaintUtil::margin(), buttonSize.height());
        layout.buttonList.append(std::move(buttonLayout));
        x += buttonLayout.buttonRect.width() + DelegatePaintUtil::margin();
    }
    return layout;
}

void MessageDelegateHelperActions::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    MessageBlockDelegateHelperBase::setRocketChatAccount(newRocketChatAccount);
}
