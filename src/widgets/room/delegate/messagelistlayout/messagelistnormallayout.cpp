/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistnormallayout.h"
#include "delegateutils/messagedelegateutils.h"
#include "model/messagemodel.h"
#include "rocketchataccount.h"
#include "room/delegate/messagedelegatehelperbase.h"
#include "room/delegate/messagedelegatehelperreactions.h"
#include "room/delegate/messagedelegatehelpertext.h"
#include "room/delegate/messagelistdelegate.h"

MessageListNormalLayout::MessageListNormalLayout(MessageListDelegate *delegate)
    : MessageListLayoutBase(delegate)
{
}

MessageListNormalLayout::~MessageListNormalLayout() = default;

MessageListLayoutBase::Layout MessageListNormalLayout::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return {};
}

QSize MessageListNormalLayout::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return {};
}
