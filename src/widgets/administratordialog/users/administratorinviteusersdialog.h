/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>

#include "libruqolawidgets_private_export.h"
class AdministratorInviteUsersWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorInviteUsersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdministratorInviteUsersDialog(QWidget *parent = nullptr);
    ~AdministratorInviteUsersDialog() override;

    Q_REQUIRED_RESULT QStringList emails() const;

private:
    AdministratorInviteUsersWidget *const mInviteUsersWidget;
};
