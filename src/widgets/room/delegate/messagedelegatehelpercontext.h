/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "delegateutils/textselectionimpl.h"
#include "messageblockdelegatehelperbase.h"
#include "messages/block/block.h"

#include <QModelIndex>
#include <QSize>

class QListView;
class QPainter;
class QRect;
class QMouseEvent;
class QStyleOptionViewItem;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperContext : public MessageBlockDelegateHelperBase
{
public:
    explicit MessageDelegateHelperContext(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageDelegateHelperContext() override;
    void draw(const Block &block, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const override;
    [[nodiscard]] QSize sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const override;
    [[nodiscard]] bool
    handleMouseEvent(const Block &block, QMouseEvent *mouseEvent, QRect blockRect, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    struct ButtonLayout {
        QString text;
        QString actionId;
        QString appId;
        QString blockId;
        QString value;
        QString url;
        QRectF buttonRect;
    };
    struct ContextLayout {
        QList<ButtonLayout> textList;
    };
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT ContextLayout layoutContext(const Block &block, const QStyleOptionViewItem &option, int attachmentsWidth) const;
};
