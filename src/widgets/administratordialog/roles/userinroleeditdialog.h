/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class UsersInRoleWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UserInRoleEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UserInRoleEditDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~UserInRoleEditDialog() override;

    [[nodiscard]] const QString &roleId() const;
    void setRoleId(const QString &newRoleId);

    [[nodiscard]] const QString &roleName() const;
    void setRoleName(const QString &newRoleName);

private:
    void readConfig();
    void writeConfig();
    UsersInRoleWidget *const mUserInRoleEditWidget;
};
