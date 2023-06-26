/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "importexportdata/importexportutils.h"
#include "libruqolawidgets_private_export.h"
#include <QWizardPage>
class ExportDataSelectAccountListWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExportDataSelectAccountPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ExportDataSelectAccountPage(QWidget *parent = nullptr);
    ~ExportDataSelectAccountPage() override;

    void setAccountList(const QVector<ImportExportUtils::AccountImportExportInfo> &lst);

    Q_REQUIRED_RESULT QVector<ImportExportUtils::AccountImportExportInfo> selectedAccounts() const;

    Q_REQUIRED_RESULT bool validatePage() override;

    Q_REQUIRED_RESULT bool isComplete() const override;

private:
    ExportDataSelectAccountListWidget *const mExportDataSelectAccountListWidget;
};
