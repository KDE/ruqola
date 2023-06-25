/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "importexportdata/importexportutils.h"
#include "libruqolawidgets_private_export.h"

#include <QListWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ExportDataSelectAccountListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ExportDataSelectAccountListWidget(QWidget *parent = nullptr);
    ~ExportDataSelectAccountListWidget() override;

    void setAccountList(const QVector<ImportExportUtils::AccountImportExportInfo> &lst);

    Q_REQUIRED_RESULT QVector<ImportExportUtils::AccountImportExportInfo> selectedAccounts() const;

private:
    enum DataAccountEnum {
        DataAccount = Qt::UserRole + 1,
    };
};
