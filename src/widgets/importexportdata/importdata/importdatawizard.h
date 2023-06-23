/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

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
    void readConfig();
    void writeConfig();
    ImportDataSelectAccountPage *const mImportDataSelectAccountPage;
    ImportDataFinishPage *const mImportDataFinishPage;
};
