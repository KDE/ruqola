/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistdelegatebase.h"
#include "delegateutils/textselection.h"
#include "delegateutils/textselectionimpl.h"

MessageListDelegateBase::MessageListDelegateBase(QObject *parent)
    : QItemDelegate{parent}
    , mTextSelectionImpl(new TextSelectionImpl)
{
    auto textSelection = mTextSelectionImpl->textSelection();
    textSelection->setTextHelperFactory(this);
    connect(textSelection, &TextSelection::repaintNeeded, this, &MessageListDelegateBase::updateView);
}

MessageListDelegateBase::~MessageListDelegateBase()
{
    delete mTextSelectionImpl;
}

void MessageListDelegateBase::clearCache()
{
    mDocumentCache.clear();
}
