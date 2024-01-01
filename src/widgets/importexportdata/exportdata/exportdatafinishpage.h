/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "importexportdata/importexportutils.h"
#include "libruqolawidgets_private_export.h"
#include <QWizardPage>
class QLabel;
class KMessageWidget;
class QPlainTextEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExportDataFinishPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ExportDataFinishPage(QWidget *parent = nullptr);
    ~ExportDataFinishPage() override;
    void setListAccounts(const QVector<ImportExportUtils::AccountImportExportInfo> &newListAccounts);
    void exportAccounts();

    [[nodiscard]] bool isComplete() const override;

private:
    [[nodiscard]] QString generateExportZipFileName() const;
    void slotExportDone();
    void slotExportFailed(const QString &msg);
    void slotExportInfo(const QString &msg);
    QVector<ImportExportUtils::AccountImportExportInfo> mListAccounts;
    QLabel *const mInfos;
    KMessageWidget *const mMessageWidget;
    QPlainTextEdit *const mDetails;
    bool mExportDone = false;
};
