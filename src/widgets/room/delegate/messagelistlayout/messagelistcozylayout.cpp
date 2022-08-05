/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistcozylayout.h"
#include "delegateutils/messagedelegateutils.h"
#include "model/messagemodel.h"
#include "rocketchataccount.h"
#include "room/delegate/messagedelegatehelperbase.h"
#include "room/delegate/messagedelegatehelperreactions.h"
#include "room/delegate/messagedelegatehelpertext.h"
#include "room/delegate/messagelistdelegate.h"

MessageListCozyLayout::MessageListCozyLayout(MessageListDelegate *delegate)
    : MessageListLayoutBase(delegate)
{
}

MessageListCozyLayout::~MessageListCozyLayout() = default;

MessageListLayoutBase::Layout MessageListCozyLayout::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return {};
}

QSize MessageListCozyLayout::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return {};
}
