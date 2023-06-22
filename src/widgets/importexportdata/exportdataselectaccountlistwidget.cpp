/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdataselectaccountlistwidget.h"
#include <QListWidgetItem>

ExportDataSelectAccountListWidget::ExportDataSelectAccountListWidget(QWidget *parent)
    : QListWidget(parent)
{
}

ExportDataSelectAccountListWidget::~ExportDataSelectAccountListWidget() = default;

void ExportDataSelectAccountListWidget::setAccountList(const QStringList &lst)
{
    for (const QString &accountName : lst) {
        // TODO
    }
}
