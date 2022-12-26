/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>

#include "roles/roleinfo.h"

#include "users/userscreatejob.h"

#include "libruqolawidgets_private_export.h"
class AdministratorAddUserWidget;
class User;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorAddUserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdministratorAddUserDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorAddUserDialog() override;

    Q_REQUIRED_RESULT RocketChatRestApi::CreateUpdateUserInfo createInfo() const;
    Q_REQUIRED_RESULT RocketChatRestApi::UpdateUserInfo updateInfo() const;
    void setUser(const User &user);
    void setRoleInfo(const QVector<RoleInfo> &info);

private:
    AdministratorAddUserWidget *const mAdministratorWidget;
};
