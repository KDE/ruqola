/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class CustomUserStatusTreeWidget;
class KTreeWidgetSearchLineWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorCustomUserStatusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorCustomUserStatusWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorCustomUserStatusWidget() override;
    void initialize();

private:
    CustomUserStatusTreeWidget *const mCustomUserStatusTreeWidget;
    KTreeWidgetSearchLineWidget *const mSearchLineWidget;
};
