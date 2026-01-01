/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class RocketChatAccount;
class AdministratorModerationReportedMessageConsoleTreeWidget;
class AdministratorModerationReportedUserConsoleTreeWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorModerationConsoleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorModerationConsoleWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorModerationConsoleWidget() override;

    void initialize();

private:
    AdministratorModerationReportedMessageConsoleTreeWidget *const mAdministratorModerationReportedMessageConsoleTreeWidget;
    AdministratorModerationReportedUserConsoleTreeWidget *const mAdministratorModerationReportedUserConsoleTreeWidget;
};
