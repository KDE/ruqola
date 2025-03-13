/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "multistaticselectlineedit.h"
#include "common/completionlistview.h"
#include "multistaticselectlineeditmodel.h"

MultiStaticSelectLineEdit::MultiStaticSelectLineEdit(QWidget *parent)
    : CompletionLineEdit(parent)
    , mMultiStaticSelectLineEditModel(new MultiStaticSelectLineEditModel(this))
{
    setCompletionModel(mMultiStaticSelectLineEditModel);
    connect(this, &MultiStaticSelectLineEdit::textChanged, this, &MultiStaticSelectLineEdit::slotSearchTextEdited);
    mCompletionListView->setTextWidget(this);
}

void MultiStaticSelectLineEdit::setUserCompletionInfos(const QList<MultiStaticSelectLineEditModel::UserCompletionInfo> &newUserCompletionInfos)
{
    mMultiStaticSelectLineEditModel->setUserCompletionInfos(newUserCompletionInfos);
}

MultiStaticSelectLineEdit::~MultiStaticSelectLineEdit() = default;

void MultiStaticSelectLineEdit::slotSearchTextEdited()
{
    // TODO
}

#include "moc_multistaticselectlineedit.cpp"
