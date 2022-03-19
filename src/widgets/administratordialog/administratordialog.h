/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class AdministratorWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdministratorDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorDialog() override;

    void initialize();

private:
    void readConfig();
    void writeConfig();
    AdministratorWidget *const mAdministratorWidget;
};
