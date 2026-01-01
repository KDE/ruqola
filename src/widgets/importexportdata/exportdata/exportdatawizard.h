/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

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
    enum class ExportDataEnum : uint8_t {
        SelectAccountPage,
        FinishPage,
    };
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCurrentIdChanged(int id);
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void loadAccountInfo();
    LIBRUQOLAWIDGETS_NO_EXPORT void exportAccounts();
    ExportDataSelectAccountPage *const mExportDataSelectAccountPage;
    ExportDataFinishPage *const mExportDataFinishPage;
};
