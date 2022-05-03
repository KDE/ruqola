/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
// class AdministratorWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdministratorSettingsDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorSettingsDialog() override;

    void initialize();

private:
    void readConfig();
    void writeConfig();
    // AdministratorWidget *const mAdministratorWidget;
};
