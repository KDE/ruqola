/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class AdministratorSettingsWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdministratorSettingsDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorSettingsDialog() override;

    void loadSettings();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    AdministratorSettingsWidget *const mAdministratorSettingsWidget;
};
