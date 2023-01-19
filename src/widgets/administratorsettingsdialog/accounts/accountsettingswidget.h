/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QSpinBox;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AccountSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit AccountSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AccountSettingsWidget() override;

    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QCheckBox *const mAllowChangeName;
    QCheckBox *const mAllowChangeUserProfile;
    QCheckBox *const mAllowChangeUserAvatar;
    QCheckBox *const mAllowChangeEmail;
    QCheckBox *const mAllowChangePassword;
    QCheckBox *const mAllowChangeEmailNotifications;
    QCheckBox *const mAllowUsersToDeleteOwnAccount;
    QCheckBox *const mAllowAnonymousWrite;
    QCheckBox *const mAllowAnonymousRead;
    QCheckBox *const mAllowUsersDeleteOwnAccount;
    QCheckBox *const mAllowPasswordChangeOauthUsers;
    QSpinBox *const mLoginExpirationInDays;
    QCheckBox *const mAllowInvisibleStatusOption;
    QCheckBox *const mForgetUserSessionWindowClose;
    QCheckBox *const mEnableCollectLog;
    QCheckBox *const mAllowCustomStatusMessage;
    QCheckBox *const mBlockFailedLoginAttemptsUsername;
    QSpinBox *const mHowManyFailedAttemptsUntilBlockUser;
    QSpinBox *const mTimeUnblockUser;
};
