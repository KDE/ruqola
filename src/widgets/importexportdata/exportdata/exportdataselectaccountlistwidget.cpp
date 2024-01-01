/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdataselectaccountlistwidget.h"
#include <QListWidgetItem>

ExportDataSelectAccountListWidget::ExportDataSelectAccountListWidget(QWidget *parent)
    : QListWidget(parent)
{
    setAlternatingRowColors(true);
    connect(this, &ExportDataSelectAccountListWidget::itemChanged, this, &ExportDataSelectAccountListWidget::selectedItemChanged);
}

ExportDataSelectAccountListWidget::~ExportDataSelectAccountListWidget() = default;

void ExportDataSelectAccountListWidget::setAccountList(const QVector<ImportExportUtils::AccountImportExportInfo> &lst)
{
    for (const auto &accountName : lst) {
        auto item = new QListWidgetItem(this);
        item->setText(accountName.accountName);
        item->setCheckState(Qt::Unchecked);
        item->setData(ExportDataSelectAccountListWidget::DataAccount, QVariant::fromValue(accountName));
    }
}

QVector<ImportExportUtils::AccountImportExportInfo> ExportDataSelectAccountListWidget::selectedAccounts() const
{
    QVector<ImportExportUtils::AccountImportExportInfo> selectAccountsList;
    const int numberOfItems(count());
    for (int i = 0; i < numberOfItems; ++i) {
        QListWidgetItem *it = item(i);
        if (it->checkState() == Qt::Checked) {
            selectAccountsList.append(it->data(ExportDataSelectAccountListWidget::DataAccount).value<ImportExportUtils::AccountImportExportInfo>());
        }
    }
    return selectAccountsList;
}

#include "moc_exportdataselectaccountlistwidget.cpp"
