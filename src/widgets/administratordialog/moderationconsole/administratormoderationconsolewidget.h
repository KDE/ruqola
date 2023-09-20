/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>
class RocketChatAccount;
class ModerationConsoleTreeWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorModerationConsoleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorModerationConsoleWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorModerationConsoleWidget() override;

private:
    ModerationConsoleTreeWidget *const mModerationConsoleTreeWidget;
    RocketChatAccount *const mRocketChatAccount;
};
