/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exportdataselectaccountpage.h"

#include "exportdataselectaccountlistwidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ExportDataSelectAccountPage::ExportDataSelectAccountPage(QWidget *parent)
    : QWizardPage(parent)
    , mExportDataSelectAccountListWidget(new ExportDataSelectAccountListWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18nc("@label:textbox", "Select Accounts:"), this);
    label->setObjectName(u"label"_s);
    label->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(label);

    mExportDataSelectAccountListWidget->setObjectName(u"mExportDataSelectAccountListWidget"_s);
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
