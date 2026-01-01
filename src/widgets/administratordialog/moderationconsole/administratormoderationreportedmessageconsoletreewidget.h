/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class ModerationReportedMessageConsoleTreeWidget;
class AdministratorModerationRangeWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorModerationReportedMessageConsoleTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorModerationReportedMessageConsoleTreeWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorModerationReportedMessageConsoleTreeWidget() override;
    void initialize();

private:
    ModerationReportedMessageConsoleTreeWidget *const mModerationConsoleTreeWidget;
    AdministratorModerationRangeWidget *const mAdministratorModerationRangeWidget;
};
