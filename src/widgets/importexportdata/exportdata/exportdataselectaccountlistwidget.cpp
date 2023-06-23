/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdataselectaccountlistwidget.h"
#include <QDebug>
#include <QListWidgetItem>

ExportDataSelectAccountListWidget::ExportDataSelectAccountListWidget(QWidget *parent)
    : QListWidget(parent)
{
}

ExportDataSelectAccountListWidget::~ExportDataSelectAccountListWidget() = default;

void ExportDataSelectAccountListWidget::setAccountList(const QStringList &lst)
{
    qDebug() << " lst " << lst;
    for (const QString &accountName : lst) {
        auto item = new QListWidgetItem(this);
        item->setText(accountName);
        item->setCheckState(Qt::Unchecked);
    }
}

QStringList ExportDataSelectAccountListWidget::selectedAccounts() const
{
    QStringList selectAccountsList;
    const int numberOfItems(count());
    for (int i = 0; i < numberOfItems; ++i) {
        QListWidgetItem *it = item(i);
        if (it->checkState() == Qt::Checked) {
            selectAccountsList.append(it->text());
        }
    }
    return selectAccountsList;
}

#include "moc_exportdataselectaccountlistwidget.cpp"
