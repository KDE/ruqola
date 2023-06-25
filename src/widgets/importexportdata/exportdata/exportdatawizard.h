/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWizard>
class ExportDataSelectAccountPage;
class ExportDataFinishPage;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExportDataWizard : public QWizard
{
    Q_OBJECT
public:
    explicit ExportDataWizard(QWidget *parent = nullptr);
    ~ExportDataWizard() override;

private:
    enum ExportDataEnum {
        SelectAccountPage,
        FinishPage,
    };
    void slotCurrentIdChanged(int id);
    void readConfig();
    void writeConfig();
    void loadAccountInfo();
    ExportDataSelectAccountPage *const mExportDataSelectAccountPage;
    ExportDataFinishPage *const mExportDataFinishPage;
};
