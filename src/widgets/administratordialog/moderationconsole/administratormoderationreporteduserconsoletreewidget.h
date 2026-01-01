/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class ModerationReportedUserConsoleTreeWidget;
class AdministratorModerationRangeWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorModerationReportedUserConsoleTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorModerationReportedUserConsoleTreeWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorModerationReportedUserConsoleTreeWidget() override;
    void initialize();

private:
    ModerationReportedUserConsoleTreeWidget *const mModerationConsoleTreeWidget;
    AdministratorModerationRangeWidget *const mAdministratorModerationRangeWidget;
};
