/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "messagedelegatehelperbase.h"
class QStyleOptionViewItem;
class QMouseEvent;
class QHelpEvent;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageBlockDelegateHelperBase : public MessageDelegateHelperBase
{
public:
    explicit MessageBlockDelegateHelperBase(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    virtual ~MessageBlockDelegateHelperBase();

    virtual void draw(const Block &block, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const = 0;
    [[nodiscard]] virtual bool
    handleMouseEvent(const Block &block, QMouseEvent *mouseEvent, QRect blocksRect, const QStyleOptionViewItem &option, const QModelIndex &index) = 0;

    virtual bool handleHelpEvent(QHelpEvent *helpEvent, QRect messageRect, const Block &block, const QStyleOptionViewItem &option);

    virtual QSize sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const = 0;
};
