/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

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

    void setAccountList(const QList<ImportExportUtils::AccountImportExportInfo> &lst);

    [[nodiscard]] QList<ImportExportUtils::AccountImportExportInfo> selectedAccounts() const;

Q_SIGNALS:
    void selectedItemChanged();

private:
    enum DataAccountEnum {
        DataAccount = Qt::UserRole + 1,
    };
};
