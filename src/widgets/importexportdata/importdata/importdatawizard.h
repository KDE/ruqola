/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWizard>
class ImportDataSelectAccountPage;
class ImportDataFinishPage;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ImportDataWizard : public QWizard
{
    Q_OBJECT
public:
    explicit ImportDataWizard(QWidget *parent = nullptr);
    ~ImportDataWizard() override;

private:
    enum ImportDataEnum {
        SelectAccountPage,
        FinishPage,
    };
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCurrentIdChanged(int id);
    ImportDataSelectAccountPage *const mImportDataSelectAccountPage;
    ImportDataFinishPage *const mImportDataFinishPage;
};
