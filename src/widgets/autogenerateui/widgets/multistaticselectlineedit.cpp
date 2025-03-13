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

void MultiStaticSelectLineEdit::setUserCompletionInfos(const QList<MultiStaticSelectLineEditModel::UserCompletionInfo> &newUserCompletionInfos)
{
    mMultiStaticSelectLineEditModel->setUserCompletionInfos(newUserCompletionInfos);
}

MultiStaticSelectLineEdit::~MultiStaticSelectLineEdit() = default;

void MultiStaticSelectLineEdit::slotSearchTextEdited()
{
    mMultiStaticSelectLineEditProxyModel->setFilterFixedString(text());
    mCompletionListView->show();
}

void MultiStaticSelectLineEdit::slotComplete(const QModelIndex &index)
{
    const QString text = index.data(MultiStaticSelectLineEditModel::Text).toString();
    const QByteArray value = index.data(MultiStaticSelectLineEditModel::Value).toByteArray();
    MultiStaticSelectLineEditModel::UserCompletionInfo info;
    info.text = text;
    info.value = QString::fromLatin1(value);
    mCompletionListView->hide();
    disconnect(this, &QLineEdit::textChanged, this, &MultiStaticSelectLineEdit::slotSearchTextEdited);
    // TODP Q_EMIT newRoomName(std::move(info));
    clear();
    connect(this, &QLineEdit::textChanged, this, &MultiStaticSelectLineEdit::slotSearchTextEdited);
}

#include "moc_multistaticselectlineedit.cpp"
