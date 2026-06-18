/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class RocketChatAccount;
class ShowBannedUsersWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowBannedUsersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ShowBannedUsersDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowBannedUsersDialog() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    ShowBannedUsersWidget *const mShowBannedUsersWidget;
    RocketChatAccount *const mRocketChatAccount;
};
