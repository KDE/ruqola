/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "messagelistlayoutbase.h"

class MessageListDelegate;
class MessageListNormalLayout : public MessageListLayoutBase
{
public:
    explicit MessageListNormalLayout(MessageListDelegate *delegate);
    ~MessageListNormalLayout() override;

    Q_REQUIRED_RESULT MessageListLayoutBase::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    Q_REQUIRED_RESULT QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
