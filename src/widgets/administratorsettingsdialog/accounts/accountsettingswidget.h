/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AccountSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit AccountSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AccountSettingsWidget() override;

    void initialize();

private:
    QCheckBox *const mAllowChangeName;
    QCheckBox *const mAllowChangeUserProfile;
    QCheckBox *const mAllowChangeUserAvatar;
    QCheckBox *const mAllowChangeEmail;
    QCheckBox *const mAllowChangePassword;
    QCheckBox *const mAllowChangeEmailNotifications;
    QCheckBox *const mAllowUsersToDeleteOwnAccount;
    QCheckBox *const mAllowAnonymousWrite;
    QCheckBox *const mAllowUsersDeleteOwnAccount;
    QCheckBox *const mAllowPasswordChangeOauthUsers;
};
