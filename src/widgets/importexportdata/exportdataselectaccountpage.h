/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWizardPage>
class ExportDataSelectAccountListWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExportDataSelectAccountPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ExportDataSelectAccountPage(QWidget *parent = nullptr);
    ~ExportDataSelectAccountPage() override;

    void setAccountList(const QStringList &lst);

private:
    ExportDataSelectAccountListWidget *const mExportDataSelectAccountListWidget;
};
