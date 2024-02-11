/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "completionlineedit.h"
#include "completionlistview.h"
#include <KLineEditEventHandler>

CompletionLineEdit::CompletionLineEdit(QWidget *parent)
    : QLineEdit(parent)
    , mCompletionListView(new CompletionListView)
{
    setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(this);

    mCompletionListView->setTextWidget(this);

    connect(mCompletionListView, &CompletionListView::complete, this, &CompletionLineEdit::complete);
}

CompletionLineEdit::~CompletionLineEdit()
{
    delete mCompletionListView;
}

void CompletionLineEdit::setCompletionModel(QAbstractItemModel *model)
{
    mCompletionListView->setModel(model);
}

#include "moc_completionlineedit.cpp"
