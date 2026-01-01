/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QTabWidget;
class AdministratorUsersWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorUsersTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorUsersTabWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorUsersTabWidget() override;

    void initialize();

private:
    QTabWidget *const mTabWidget;
    AdministratorUsersWidget *const mAllUsers;
    AdministratorUsersWidget *const mPendingUsers;
    AdministratorUsersWidget *const mDeactivatedUsers;
    AdministratorUsersWidget *const mActivatedUsers;
};
