/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "multistaticselectlineedit.h"
#include "common/completionlistview.h"
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
    // Hide after assign infos
    mCompletionListView->setVisible(false);
}

void MultiStaticSelectLineEdit::setInitialValues(const QStringList &lst)
{
    QList<MultiStaticSelectLineEditModel::SelectItemCompletionInfo> infoLst;
    for (const auto &value : lst) {
        const MultiStaticSelectLineEditModel::SelectItemCompletionInfo info = mMultiStaticSelectLineEditModel->itemCompletionInfo(value);
        if (info.isValid()) {
            infoLst.append(info);
        }
    }
    if (!infoLst.isEmpty()) {
        setCurrentSelectItems(std::move(infoLst));
    }
}

void MultiStaticSelectLineEdit::setCurrentSelectItems(const QList<MultiStaticSelectLineEditModel::SelectItemCompletionInfo> &newUserCompletionInfos)
{
    for (const auto &info : newUserCompletionInfos) {
        Q_EMIT addSelectedItem(info);
    }
}

void MultiStaticSelectLineEdit::slotSearchTextEdited()
{
    const QString str = text().trimmed();
    mMultiStaticSelectLineEditProxyModel->setFilterFixedString(str);
    mCompletionListView->setVisible(!str.isEmpty());
}

void MultiStaticSelectLineEdit::slotComplete(const QModelIndex &index)
{
    const QString text = index.data(MultiStaticSelectLineEditModel::Text).toString();
    const QByteArray value = index.data(MultiStaticSelectLineEditModel::Value).toByteArray();
    const MultiStaticSelectLineEditModel::SelectItemCompletionInfo info{
        .text = text,
        .value = QString::fromLatin1(value),
    };
    mCompletionListView->hide();
    if (info.isValid()) {
        disconnect(this, &QLineEdit::textChanged, this, &MultiStaticSelectLineEdit::slotSearchTextEdited);
        Q_EMIT addSelectedItem(std::move(info));
        clear();
        connect(this, &QLineEdit::textChanged, this, &MultiStaticSelectLineEdit::slotSearchTextEdited);
    }
}

#include "moc_multistaticselectlineedit.cpp"
