/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWizardPage>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ExportDataFinishPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ExportDataFinishPage(QWidget *parent = nullptr);
    ~ExportDataFinishPage() override;
};
