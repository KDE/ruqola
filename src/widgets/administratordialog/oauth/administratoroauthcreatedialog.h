/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QDialog>

#include "libruqolawidgets_private_export.h"
class AdministratorOauthCreateWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorOauthCreateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdministratorOauthCreateDialog(QWidget *parent = nullptr);
    ~AdministratorOauthCreateDialog() override;

private:
    void readConfig();
    void writeConfig();
    AdministratorOauthCreateWidget *const mCreateWidget;
};
