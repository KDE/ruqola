/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>

AccountSettingsWidget::AccountSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mAllowChangeName(new QCheckBox(i18n("Allow Name Change"), this))
    , mAllowChangeUserProfile(new QCheckBox(i18n("Allow User Profile Change"), this))
    , mAllowChangeUserAvatar(new QCheckBox(i18n("Allow User Avatar Change"), this))
    , mAllowChangeEmail(new QCheckBox(i18n("Allow Email Change"), this))
    , mAllowChangePassword(new QCheckBox(i18n("Allow Password Change"), this))
    , mAllowChangeEmailNotifications(new QCheckBox(i18n("Allow Email Notifications"), this))
    , mAllowUsersToDeleteOwnAccount(new QCheckBox(i18n("Allow Users to Delete Own Account"), this))
    , mAllowAnonymousWrite(new QCheckBox(i18n("Allow Anonymous Write"), this))
    , mAllowAnonymousRead(new QCheckBox(i18n("Allow Anonymous Read"), this))
    , mAllowUsersDeleteOwnAccount(new QCheckBox(i18n("Allow Users to Delete Own Account"), this))
    , mAllowPasswordChangeOauthUsers(new QCheckBox(i18n("Allow Password Change for OAuth Users"), this))
    , mLoginExpirationInDays(new QSpinBox(this))
    , mAllowInvisibleStatusOption(new QCheckBox(i18n("Allow Invisible status option"), this))
    , mForgetUserSessionWindowClose(new QCheckBox(i18n("Forget User Session on Window Close"), this))
    , mEnableCollectLog(new QCheckBox(i18n("Enable collect log in data"), this))
    , mAllowCustomStatusMessage(new QCheckBox(i18n("Allow Custom Status Message"), this))
    , mBlockFailedLoginAttemptsUsername(new QCheckBox(i18n("Block Failed Login Attempts by Username"), this))
    , mHowManyFailedAttemptsUntilBlockUser(new QSpinBox(this))
    , mTimeUnblockUser(new QSpinBox(this))
    , mSendEmailUserWhenUserActivated(new QCheckBox(i18n("Send Email to User when User is Activated"), this))
    , mSendEmailUserWhenUserDeactivated(new QCheckBox(i18n("Send Email to User when User is Deactivated"), this))
{
    mAllowChangeName->setObjectName(QStringLiteral("mAllowChangeName"));
    mMainLayout->addWidget(mAllowChangeName);
    connectCheckBox(mAllowChangeName, QStringLiteral("Accounts_AllowRealNameChange"));

    mAllowCustomStatusMessage->setObjectName(QStringLiteral("mAllowCustomStatusMessage"));
    mMainLayout->addWidget(mAllowCustomStatusMessage);
    connectCheckBox(mAllowCustomStatusMessage, QStringLiteral("Accounts_AllowUserStatusMessageChange"));

    mAllowChangeUserProfile->setObjectName(QStringLiteral("mAllowChangeUserProfile"));
    mMainLayout->addWidget(mAllowChangeUserProfile);
    connectCheckBox(mAllowChangeUserProfile, QStringLiteral("Accounts_AllowUserProfileChange"));

    mAllowChangeUserAvatar->setObjectName(QStringLiteral("mAllowChangeUserAvatar"));
    mMainLayout->addWidget(mAllowChangeUserAvatar);
    connectCheckBox(mAllowChangeUserAvatar, QStringLiteral("Accounts_AllowUserAvatarChange"));

    mAllowChangeEmail->setObjectName(QStringLiteral("mAllowChangeEmail"));
    mMainLayout->addWidget(mAllowChangeEmail);
    connectCheckBox(mAllowChangeEmail, QStringLiteral("Accounts_AllowEmailChange"));

    mAllowChangePassword->setObjectName(QStringLiteral("mAllowChangePassword"));
    mMainLayout->addWidget(mAllowChangePassword);
    connectCheckBox(mAllowChangePassword, QStringLiteral("Accounts_AllowPasswordChange"));

    mAllowChangeEmailNotifications->setObjectName(QStringLiteral("mAllowChangeEmailNotifications"));
    mMainLayout->addWidget(mAllowChangeEmailNotifications);
    connectCheckBox(mAllowChangeEmailNotifications, QStringLiteral("Accounts_AllowEmailNotifications"));

    mAllowUsersToDeleteOwnAccount->setObjectName(QStringLiteral("mAllowUsersToDeleteOwnAccount"));
    mMainLayout->addWidget(mAllowUsersToDeleteOwnAccount);
    connectCheckBox(mAllowUsersToDeleteOwnAccount, QStringLiteral("Accounts_AllowDeleteOwnAccount"));

    mAllowAnonymousWrite->setObjectName(QStringLiteral("mAllowAnonymousWrite"));
    mMainLayout->addWidget(mAllowAnonymousWrite);
    connectCheckBox(mAllowAnonymousWrite, QStringLiteral("Accounts_AllowAnonymousWrite"));

    mAllowAnonymousRead->setObjectName(QStringLiteral("mAllowAnonymousRead"));
    mMainLayout->addWidget(mAllowAnonymousRead);
    connectCheckBox(mAllowAnonymousRead, QStringLiteral("Accounts_AllowAnonymousRead"));

    mAllowUsersDeleteOwnAccount->setObjectName(QStringLiteral("mAllowUsersDeleteOwnAccount"));
    mMainLayout->addWidget(mAllowUsersDeleteOwnAccount);
    connectCheckBox(mAllowUsersDeleteOwnAccount, QStringLiteral("Accounts_AllowDeleteOwnAccount"));

    mAllowPasswordChangeOauthUsers->setObjectName(QStringLiteral("mAllowPasswordChangeOauthUsers"));
    mMainLayout->addWidget(mAllowPasswordChangeOauthUsers);
    connectCheckBox(mAllowPasswordChangeOauthUsers, QStringLiteral("Accounts_AllowPasswordChangeForOAuthUsers"));

    mLoginExpirationInDays->setObjectName(QStringLiteral("mLoginExpirationInDays"));
    addSpinbox(i18n("Login Expiration in Days"), mLoginExpirationInDays, QStringLiteral("Accounts_LoginExpiration"));

    mAllowInvisibleStatusOption->setObjectName(QStringLiteral("mAllowInvisibleStatusOption"));
    mMainLayout->addWidget(mAllowInvisibleStatusOption);
    connectCheckBox(mAllowInvisibleStatusOption, QStringLiteral("Accounts_AllowInvisibleStatusOption"));

    mForgetUserSessionWindowClose->setObjectName(QStringLiteral("mForgetUserSessionWindowClose"));
    mMainLayout->addWidget(mForgetUserSessionWindowClose);
    connectCheckBox(mForgetUserSessionWindowClose, QStringLiteral("Accounts_ForgetUserSessionOnWindowClose"));

    auto failedLoginAttemptsLabel = createBoldLabel(i18n("Failed Login Attempts"));
    failedLoginAttemptsLabel->setObjectName(QStringLiteral("failedLoginAttemptsLabel"));
    mMainLayout->addWidget(failedLoginAttemptsLabel);

    mEnableCollectLog->setObjectName(QStringLiteral("mEnableCollectLog"));
    mMainLayout->addWidget(mEnableCollectLog);
    connectCheckBox(mEnableCollectLog, QStringLiteral("Block_Multiple_Failed_Logins_Enabled"));

    mBlockFailedLoginAttemptsUsername->setObjectName(QStringLiteral("mBlockFailedLoginAttemptsUsername"));
    mMainLayout->addWidget(mBlockFailedLoginAttemptsUsername);
    connectCheckBox(mBlockFailedLoginAttemptsUsername, QStringLiteral("Block_Multiple_Failed_Logins_By_User"));

    mHowManyFailedAttemptsUntilBlockUser->setObjectName(QStringLiteral("mBlockFailedLoginAttemptsUsername"));
    addSpinbox(i18n("How Many Failed Attempts Until Block by User"),
               mHowManyFailedAttemptsUntilBlockUser,
               QStringLiteral("Block_Multiple_Failed_Logins_Attempts_Until_Block_by_User"));

    mTimeUnblockUser->setObjectName(QStringLiteral("mTimeUnblockUser"));
    addSpinbox(i18n("Time to unblock User (In Minutes)"), mTimeUnblockUser, QStringLiteral("Block_Multiple_Failed_Logins_Time_To_Unblock_By_User_In_Minutes"));

    auto regislationLabel = createBoldLabel(i18n("Registration"));
    regislationLabel->setObjectName(QStringLiteral("regislationLabel"));
    mMainLayout->addWidget(regislationLabel);

    mSendEmailUserWhenUserActivated->setObjectName(QStringLiteral("mSendEmailUserWhenUserActivated"));
    mMainLayout->addWidget(mSendEmailUserWhenUserActivated);
    connectCheckBox(mSendEmailUserWhenUserActivated, QStringLiteral("Accounts_Send_Email_When_Activating"));

    mSendEmailUserWhenUserDeactivated->setObjectName(QStringLiteral("mSendEmailUserWhenUserDeactivated"));
    mMainLayout->addWidget(mSendEmailUserWhenUserDeactivated);
    connectCheckBox(mSendEmailUserWhenUserDeactivated, QStringLiteral("Accounts_Send_Email_When_Deactivating"));
}

AccountSettingsWidget::~AccountSettingsWidget() = default;

void AccountSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mAllowChangeName, mapSettings, true);
    initializeWidget(mAllowChangeUserProfile, mapSettings, true);
    initializeWidget(mAllowChangeUserAvatar, mapSettings, true);
    initializeWidget(mAllowChangeEmail, mapSettings, true);
    initializeWidget(mAllowChangePassword, mapSettings, true);
    initializeWidget(mAllowChangeEmailNotifications, mapSettings, true);
    initializeWidget(mAllowUsersToDeleteOwnAccount, mapSettings, true);
    initializeWidget(mAllowAnonymousWrite, mapSettings, false);
    initializeWidget(mAllowAnonymousRead, mapSettings, false);
    initializeWidget(mAllowUsersDeleteOwnAccount, mapSettings, false);
    initializeWidget(mAllowPasswordChangeOauthUsers, mapSettings, false);
    initializeWidget(mLoginExpirationInDays, mapSettings, 90);
    initializeWidget(mAllowInvisibleStatusOption, mapSettings, true);
    initializeWidget(mForgetUserSessionWindowClose, mapSettings, false);
    initializeWidget(mEnableCollectLog, mapSettings, false);
    initializeWidget(mAllowCustomStatusMessage, mapSettings, true);
    initializeWidget(mBlockFailedLoginAttemptsUsername, mapSettings, true);
    initializeWidget(mHowManyFailedAttemptsUntilBlockUser, mapSettings, 10);
    initializeWidget(mTimeUnblockUser, mapSettings, 5);
    initializeWidget(mSendEmailUserWhenUserActivated, mapSettings, true);
    initializeWidget(mSendEmailUserWhenUserDeactivated, mapSettings, true);
}
