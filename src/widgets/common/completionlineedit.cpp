/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "completionlineedit.h"
#if HAVE_TEXTADDONSWIDGETS_COMPLETIONLINEEDIT
#include <TextAddonsWidgets/CompletionListView>
#else
#include "completionlistview.h"
#endif
#include <KLineEditEventHandler>

CompletionLineEdit::CompletionLineEdit(QWidget *parent)
    : QLineEdit(parent)
    , mCompletionListView(new
#if HAVE_TEXTADDONSWIDGETS_COMPLETIONLINEEDIT
                          TextAddonsWidgets::
#endif
                              CompletionListView)
{
    setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(this);

    mCompletionListView->setTextWidget(this);

    connect(mCompletionListView,
            &
#if HAVE_TEXTADDONSWIDGETS_COMPLETIONLINEEDIT
            TextAddonsWidgets::
#endif
                CompletionListView::complete,
            this,
            &CompletionLineEdit::complete);
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
