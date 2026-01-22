/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelperactions.h"

#include "common/delegatepaintutil.h"
#include "dialogs/showvideodialog.h"
#include "misc/messageattachmentdownloadandsavejob.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>

using namespace Qt::Literals::StringLiterals;
MessageAttachmentDelegateHelperActions::MessageAttachmentDelegateHelperActions(RocketChatAccount *account,
                                                                               QListView *view,
                                                                               TextSelectionImpl *textSelectionImpl)
    : MessageDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageAttachmentDelegateHelperActions::~MessageAttachmentDelegateHelperActions() = default;

void MessageAttachmentDelegateHelperActions::draw(const MessageAttachmentActions &act,
                                                  QPainter *painter,
                                                  QRect messageRect,
                                                  [[maybe_unused]] const QModelIndex &index,
                                                  const QStyleOptionViewItem &option) const
{
    const ActionsLayout layout = layoutActions(act, option, messageRect.width());
    for (const auto &button : std::as_const(layout.buttonList)) {
        // Draw button
        const QPen origPen = painter->pen();
        const QBrush origBrush = painter->brush();
        const QPen buttonPen(option.palette.color(QPalette::Highlight).darker());
        QColor backgroundColor = option.palette.color(QPalette::Highlight);
        backgroundColor.setAlpha(60);
        const QBrush buttonBrush(backgroundColor);
        const QRectF buttonRect = button.buttonRect.translated(messageRect.topLeft());
        // Rounded rect
        painter->setPen(buttonPen);
        painter->setBrush(buttonBrush);
        painter->drawRoundedRect(buttonRect, 5, 5);
        painter->setBrush(origBrush);
        painter->setPen(origPen);
        const QRectF r = buttonRect.adjusted((buttonRect.width() - button.buttonRect.width()) / 2, 0, 0, 0);
        painter->drawText(r, Qt::AlignVCenter | Qt::AlignHCenter, button.text);
    }
}

QSize MessageAttachmentDelegateHelperActions::sizeHint(const MessageAttachmentActions &act,
                                                       const QModelIndex &index,
                                                       int maxWidth,
                                                       const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index);
    // TODO alignment
    const ActionsLayout layout = layoutActions(act, option, maxWidth);
    if (layout.buttonList.isEmpty()) {
        return {};
    }
    const int height = layout.buttonList.at(0).buttonRect.height() + DelegatePaintUtil::margin();
    const auto buttons = layout.buttonList;
    int width = 0;
    for (const auto &b : buttons) {
        width += b.buttonRect.width();
    }
    return {qMax(0, static_cast<int>(width)), height};
}

bool MessageAttachmentDelegateHelperActions::handleMouseEvent(const MessageAttachmentActions &act,
                                                              QMouseEvent *mouseEvent,
                                                              QRect attachmentsRect,
                                                              const QStyleOptionViewItem &option,
                                                              const QModelIndex &index)
{
    return false;
}

MessageAttachmentDelegateHelperActions::ActionsLayout
MessageAttachmentDelegateHelperActions::layoutActions(const MessageAttachmentActions &act, const QStyleOptionViewItem &option, int attachmentsWidth) const
{
    ActionsLayout layout;

    qreal x = 0;
    qreal y = 0;
    const MessageAttachmentActions::AlignmentButton alignment = act.alignment();
    const bool horizontal = (alignment == MessageAttachmentActions::AlignmentButton::Horizontal);
    const auto actions = act.actions();
    for (const auto &act : actions) {
        ButtonLayout buttonLayout;
        buttonLayout.text = act.text();
        buttonLayout.message = act.msg();
        const QSize buttonSize = option.fontMetrics.size(Qt::TextSingleLine, buttonLayout.text);
        if (horizontal) {
            buttonLayout.buttonRect = QRectF(x, y, buttonSize.width() + 2 * DelegatePaintUtil::margin(), buttonSize.height());
            x += buttonLayout.buttonRect.width() + DelegatePaintUtil::margin();
        } else {
            buttonLayout.buttonRect = QRectF(x, y, buttonSize.width() + 2 * DelegatePaintUtil::margin(), buttonSize.height());
            y += buttonLayout.buttonRect.height() + DelegatePaintUtil::margin();
        }
        layout.buttonList.append(std::move(buttonLayout));
    }
    return layout;
}
