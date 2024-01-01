/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "messagelistlayoutbase.h"
class MessageListDelegate;
class MessageListCompactLayout : public MessageListLayoutBase
{
public:
    explicit MessageListCompactLayout(MessageListDelegate *delegate);
    ~MessageListCompactLayout() override;

    [[nodiscard]] MessageListLayoutBase::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
