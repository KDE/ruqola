/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWizard>

class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateVideoMessageWizard : public QWizard
{
    Q_OBJECT
public:
    explicit CreateVideoMessageWizard(QWidget *parent = nullptr);
    ~CreateVideoMessageWizard() override;
};
