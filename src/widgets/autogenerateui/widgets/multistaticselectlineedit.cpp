/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "multistaticselectlineedit.h"
#include "common/completionlistview.h"
#include "multistaticselectlineeditmodel.h"
#include "multistaticselectlineeditproxymodel.h"

MultiStaticSelectLineEdit::MultiStaticSelectLineEdit(QWidget *parent)
    : CompletionLineEdit(parent)
    , mMultiStaticSelectLineEditModel(new MultiStaticSelectLineEditModel(this))
    , mMultiStaticSelectLineEditProxyModel(new MultiStaticSelectLineEditProxyModel(this))
{
    mMultiStaticSelectLineEditProxyModel->setSourceModel(mMultiStaticSelectLineEditModel);
    setCompletionModel(mMultiStaticSelectLineEditProxyModel);
    connect(this, &MultiStaticSelectLineEdit::textChanged, this, &MultiStaticSelectLineEdit::slotSearchTextEdited);
    connect(this, &MultiStaticSelectLineEdit::complete, this, &MultiStaticSelectLineEdit::slotComplete);
    mCompletionListView->setTextWidget(this);
}

MultiStaticSelectLineEdit::~MultiStaticSelectLineEdit() = default;

void MultiStaticSelectLineEdit::setSelectItemCompletionInfos(const QList<MultiStaticSelectLineEditModel::SelectItemCompletionInfo> &newUserCompletionInfos)
{
    mMultiStaticSelectLineEditModel->setUserCompletionInfos(newUserCompletionInfos);
}

void MultiStaticSelectLineEdit::slotSearchTextEdited()
{
    mMultiStaticSelectLineEditProxyModel->setFilterFixedString(text());
    mCompletionListView->show();
}

void MultiStaticSelectLineEdit::slotComplete(const QModelIndex &index)
{
    const QString text = index.data(MultiStaticSelectLineEditModel::Text).toString();
    const QByteArray value = index.data(MultiStaticSelectLineEditModel::Value).toByteArray();
    MultiStaticSelectLineEditModel::SelectItemCompletionInfo info;
    info.text = text;
    info.value = QString::fromLatin1(value);
    mCompletionListView->hide();
    if (info.isValid()) {
        disconnect(this, &QLineEdit::textChanged, this, &MultiStaticSelectLineEdit::slotSearchTextEdited);
        Q_EMIT addSelectedItem(std::move(info));
        clear();
        connect(this, &QLineEdit::textChanged, this, &MultiStaticSelectLineEdit::slotSearchTextEdited);
    }
}

#include "moc_multistaticselectlineedit.cpp"
