/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelpercontext.h"
#include "autogenerateui/autogenerateinteractionui.h"
#include "common/delegatepaintutil.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QListView>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>

MessageDelegateHelperContext::MessageDelegateHelperContext(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageBlockDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageDelegateHelperContext::~MessageDelegateHelperContext() = default;

void MessageDelegateHelperContext::draw(const Block &block,
                                        QPainter *painter,
                                        QRect blockRect,
                                        const QModelIndex &index,
                                        const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const ContextLayout layout = layoutContext(block, option, blockRect.width());
    for (const auto &button : std::as_const(layout.textList)) {
        const QRectF textRect = button.buttonRect.translated(blockRect.topLeft());
        const QRectF r = textRect.adjusted((textRect.width() - button.buttonRect.width()) / 2, 0, 0, 0);
        painter->drawText(r, Qt::AlignVCenter | Qt::AlignHCenter, button.text);
    }
}

QSize MessageDelegateHelperContext::sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const ContextLayout layout = layoutContext(block, option, maxWidth);
    if (layout.textList.isEmpty()) {
        return {};
    }
    const int height = layout.textList.at(0).buttonRect.height() + DelegatePaintUtil::margin();
    const auto buttons = layout.textList;
    int width = 0;
    for (const auto &b : buttons) {
        width += b.buttonRect.width();
    }
    return {qMax(0, static_cast<int>(width)), height};
}

bool MessageDelegateHelperContext::handleMouseEvent(const Block &block,
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

MessageDelegateHelperContext::ContextLayout
MessageDelegateHelperContext::layoutContext(const Block &block, const QStyleOptionViewItem &option, int blockRectWidth) const
{
    Q_UNUSED(blockRectWidth)
    ContextLayout layout;

    qreal x = 0;
    const auto actions = block.blockActions();
    for (const auto &act : actions) {
        ButtonLayout buttonLayout;
        buttonLayout.text = act.text();
        buttonLayout.actionId = act.actionId();
        buttonLayout.appId = block.appId();
        buttonLayout.value = act.value();
        buttonLayout.blockId = act.blockId();
        buttonLayout.url = act.url();
        const QSize buttonSize = option.fontMetrics.size(Qt::TextSingleLine, buttonLayout.text);
        buttonLayout.buttonRect = QRectF(x, 0, buttonSize.width() + 2 * DelegatePaintUtil::margin(), buttonSize.height());
        layout.textList.append(std::move(buttonLayout));
        x += buttonLayout.buttonRect.width() + DelegatePaintUtil::margin();
    }
    return layout;
}
