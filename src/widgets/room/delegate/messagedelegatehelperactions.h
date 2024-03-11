/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "delegateutils/textselectionimpl.h"
#include "messageblockdelegatehelperbase.h"
#include "messages/block.h"

#include <QIcon>
#include <QModelIndex>
#include <QSize>

class QListView;
class QPainter;
class QRect;
class QMouseEvent;
class QStyleOptionViewItem;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperActions : public MessageBlockDelegateHelperBase
{
public:
    explicit MessageDelegateHelperActions(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageDelegateHelperActions() override;
    void draw(const Block &block, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const override;
    [[nodiscard]] QSize sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const override;
    [[nodiscard]] bool
    handleMouseEvent(const Block &block, QMouseEvent *mouseEvent, QRect blockRect, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    [[nodiscard]] bool handleHelpEvent(QHelpEvent *helpEvent, QRect blockRect, const Block &block, const QStyleOptionViewItem &option) override;

    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount) override;

private:
    struct ButtonLayout {
        QString text;
        QRectF buttonRect;
        // TODO add identifier ?
    };
    struct ActionsLayout {
        QList<ButtonLayout> buttonList;
    };
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QPoint adaptMousePosition(const QPoint &pos,
                                                                       const Block &block,
                                                                       QRect attachmentsRect,
                                                                       const QStyleOptionViewItem &option);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT ActionsLayout layoutActions(const Block &block, const QStyleOptionViewItem &option, int attachmentsWidth) const;
};
