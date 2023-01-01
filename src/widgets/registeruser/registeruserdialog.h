/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "users/registeruserjob.h"
#include <QDialog>
class RegisterUserWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RegisterUserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RegisterUserDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~RegisterUserDialog() override;
    Q_REQUIRED_RESULT RocketChatRestApi::RegisterUserJob::RegisterUserInfo registerUserInfo() const;

private:
    void readConfig();
    void writeConfig();
    RegisterUserWidget *const mRegisterUserWidget;
    RocketChatAccount *const mRocketChatAccount;
};
