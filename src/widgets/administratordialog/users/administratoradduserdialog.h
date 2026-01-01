/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>

#include "createupdateuserinfo.h"

#include "libruqolawidgets_private_export.h"
class AdministratorAddUserWidget;
class User;
class RocketChatAccount;
class RoleInfo;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorAddUserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdministratorAddUserDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorAddUserDialog() override;

    [[nodiscard]] RocketChatRestApi::CreateUpdateUserInfo createInfo() const;
    [[nodiscard]] RocketChatRestApi::UpdateUserInfo updateInfo() const;
    void setUser(const User &user);
    void setRolesInfo(const QList<RoleInfo> &info);

private:
    AdministratorAddUserWidget *const mAdministratorWidget;
};
