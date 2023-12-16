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

    [[nodiscard]] MessageListLayoutBase::Layout doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    [[nodiscard]] QRect iconRect(int iconIndex, int senderX, int iconPosition, int iconSize, int margin, int avatarWidth) const;
};
