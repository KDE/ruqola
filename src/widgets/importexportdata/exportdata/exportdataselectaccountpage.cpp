/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdataselectaccountpage.h"
#include "exportdataselectaccountlistwidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

ExportDataSelectAccountPage::ExportDataSelectAccountPage(QWidget *parent)
    : QWizardPage(parent)
    , mExportDataSelectAccountListWidget(new ExportDataSelectAccountListWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    auto label = new QLabel(i18n("Select Accounts:"), this);
    label->setObjectName(QStringLiteral("label"));
    mainLayout->addWidget(label);

    mExportDataSelectAccountListWidget->setObjectName(QStringLiteral("mExportDataSelectAccountListWidget"));
    mainLayout->addWidget(mExportDataSelectAccountListWidget);
    connect(mExportDataSelectAccountListWidget, &ExportDataSelectAccountListWidget::selectedItemChanged, this, [this]() {
        Q_EMIT completeChanged();
    });
}

ExportDataSelectAccountPage::~ExportDataSelectAccountPage() = default;

void ExportDataSelectAccountPage::setAccountList(const QList<ImportExportUtils::AccountImportExportInfo> &lst)
{
    mExportDataSelectAccountListWidget->setAccountList(lst);
}

QList<ImportExportUtils::AccountImportExportInfo> ExportDataSelectAccountPage::selectedAccounts() const
{
    return mExportDataSelectAccountListWidget->selectedAccounts();
}

bool ExportDataSelectAccountPage::validatePage()
{
    return !mExportDataSelectAccountListWidget->selectedAccounts().isEmpty();
}

bool ExportDataSelectAccountPage::isComplete() const
{
    return !mExportDataSelectAccountListWidget->selectedAccounts().isEmpty();
}

#include "moc_exportdataselectaccountpage.cpp"
