/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "users/registeruserjob.h"
#include <QDialog>

class RegisterUserWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT RegisterUserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RegisterUserDialog(QWidget *parent = nullptr);
    ~RegisterUserDialog() override;
    [[nodiscard]] RocketChatRestApi::RegisterUserJob::RegisterUserInfo registerUserInfo() const;

Q_SIGNALS:
    void registerNewAccount();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    RegisterUserWidget *const mRegisterUserWidget;
};
