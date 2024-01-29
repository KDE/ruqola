/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class MyAccountConfigureWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MyAccountConfigureDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MyAccountConfigureDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~MyAccountConfigureDialog() override;

    void initialize();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAccept();
    MyAccountConfigureWidget *const mMyAccountConfigWidget;
};
