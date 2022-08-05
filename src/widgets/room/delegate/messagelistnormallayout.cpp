/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistnormallayout.h"
#include "delegateutils/messagedelegateutils.h"
#include "messagedelegatehelperbase.h"
#include "messagedelegatehelperreactions.h"
#include "messagedelegatehelpertext.h"
#include "messagelistdelegate.h"
#include "model/messagemodel.h"
#include "rocketchataccount.h"

MessageListNormalLayout::MessageListNormalLayout(MessageListDelegate *delegate)
    : MessageListLayoutBase(delegate)
{
}

MessageListNormalLayout::~MessageListNormalLayout() = default;

MessageListLayoutBase::Layout MessageListNormalLayout::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return {};
}
