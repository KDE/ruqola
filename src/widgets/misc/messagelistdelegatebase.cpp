/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistdelegatebase.h"
#include "delegateutils/textselectionimpl.h"

MessageListDelegateBase::MessageListDelegateBase(QObject *parent)
    : QItemDelegate{parent}
    , mTextSelectionImpl(new TextSelectionImpl)
{
}

MessageListDelegateBase::~MessageListDelegateBase()
{
    delete mTextSelectionImpl;
}
