/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class ManageDeviceWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ManageDeviceDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ManageDeviceDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ManageDeviceDialog() override;

private:
    void readConfig();
    void writeConfig();
    ManageDeviceWidget *const mManageDeviceWidget;
};
