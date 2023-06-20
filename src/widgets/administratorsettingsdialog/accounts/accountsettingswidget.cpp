/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
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
    , mHowManyFailedAttemptsUntilBlockByIP(new QSpinBox(this))
    , mSendEmailUserWhenUserActivated(new QCheckBox(i18n("Send Email to User when User is Activated"), this))
    , mSendEmailUserWhenUserDeactivated(new QCheckBox(i18n("Send Email to User when User is Deactivated"), this))
    , mRequireNameSignup(new QCheckBox(i18n("Require Name For Signup"), this))
    , mRequirePasswordConfirmation(new QCheckBox(i18n("Require Password Confirmation"), this))
    , mOnlyAllowVerifiedUsersLogin(new QCheckBox(i18n("Only Allow Verified Users to Login"), this))
    , mMarkEmailExternalAccountsVerified(new QCheckBox(i18n("Mark Email for External Accounts Verified"), this))
    , mManualApproveNewUser(new QCheckBox(i18n("Manually Approve New Users"), this))
    , mDefaultUsernamePrefixSuggestion(new QLineEdit(this))
    , mAllowedDomainsList(new QLineEdit(this))
    , mBlockedDomainsList(new QLineEdit(this))
    , mBlockedUsernameList(new QLineEdit(this))
    , mUseDefaultBlockedDomainsList(new QCheckBox(i18n("Use Default Blocked Domains List"), this))
    , mUseDNSDomainCheck(new QCheckBox(i18n("Use DNS Domain Check"), this))
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

    mHowManyFailedAttemptsUntilBlockUser->setObjectName(QStringLiteral("mHowManyFailedAttemptsUntilBlockUser"));
    addSpinbox(i18n("How Many Failed Attempts Until Block by User"),
               mHowManyFailedAttemptsUntilBlockUser,
               QStringLiteral("Block_Multiple_Failed_Logins_Attempts_Until_Block_by_User"));

    mTimeUnblockUser->setObjectName(QStringLiteral("mTimeUnblockUser"));
    addSpinbox(i18n("Time to unblock User (In Minutes)"), mTimeUnblockUser, QStringLiteral("Block_Multiple_Failed_Logins_Time_To_Unblock_By_User_In_Minutes"));

    mHowManyFailedAttemptsUntilBlockByIP->setObjectName(QStringLiteral("mHowManyFailedAttemptsUntilBlockByIP"));
    addSpinbox(i18n("How many failed attempts until block by IP"),
               mHowManyFailedAttemptsUntilBlockByIP,
               QStringLiteral("Block_Multiple_Failed_Logins_Attempts_Until_Block_By_Ip"));

    auto regislationLabel = createBoldLabel(i18n("Registration"));
    regislationLabel->setObjectName(QStringLiteral("regislationLabel"));
    mMainLayout->addWidget(regislationLabel);

    mSendEmailUserWhenUserActivated->setObjectName(QStringLiteral("mSendEmailUserWhenUserActivated"));
    mMainLayout->addWidget(mSendEmailUserWhenUserActivated);
    connectCheckBox(mSendEmailUserWhenUserActivated, QStringLiteral("Accounts_Send_Email_When_Activating"));

    mSendEmailUserWhenUserDeactivated->setObjectName(QStringLiteral("mSendEmailUserWhenUserDeactivated"));
    mMainLayout->addWidget(mSendEmailUserWhenUserDeactivated);
    connectCheckBox(mSendEmailUserWhenUserDeactivated, QStringLiteral("Accounts_Send_Email_When_Deactivating"));

    mRequireNameSignup->setObjectName(QStringLiteral("mRequireNameSignup"));
    mMainLayout->addWidget(mRequireNameSignup);
    connectCheckBox(mRequireNameSignup, QStringLiteral("Accounts_RequireNameForSignUp"));

    mRequirePasswordConfirmation->setObjectName(QStringLiteral("mRequirePasswordConfirmation"));
    mMainLayout->addWidget(mRequirePasswordConfirmation);
    connectCheckBox(mRequirePasswordConfirmation, QStringLiteral("Accounts_RequirePasswordConfirmation"));

    mOnlyAllowVerifiedUsersLogin->setObjectName(QStringLiteral("mOnlyAllowVerifiedUsersLogin"));
    mMainLayout->addWidget(mOnlyAllowVerifiedUsersLogin);
    mOnlyAllowVerifiedUsersLogin->setToolTip(i18n("Make sure you have correct SMTP settings to use this feature"));
    connectCheckBox(mOnlyAllowVerifiedUsersLogin, QStringLiteral("Accounts_EmailVerification"));

    mMarkEmailExternalAccountsVerified->setObjectName(QStringLiteral("mMarkEmailExternalAccountsVerified"));
    mMainLayout->addWidget(mMarkEmailExternalAccountsVerified);
    connectCheckBox(mMarkEmailExternalAccountsVerified, QStringLiteral("Accounts_Verify_Email_For_External_Accounts"));

    mManualApproveNewUser->setObjectName(QStringLiteral("mManualApproveNewUser"));
    mMainLayout->addWidget(mManualApproveNewUser);
    connectCheckBox(mManualApproveNewUser, QStringLiteral("Accounts_ManuallyApproveNewUsers"));

    mDefaultUsernamePrefixSuggestion->setObjectName(QStringLiteral("mDefaultUsernamePrefixSuggestion"));
    addLineEdit(i18n("Default Username Prefix Suggestion"), mDefaultUsernamePrefixSuggestion, QStringLiteral("Accounts_DefaultUsernamePrefixSuggestion"));

    mAllowedDomainsList->setObjectName(QStringLiteral("mAllowedDomainsList"));
    mAllowedDomainsList->setToolTip(i18n("Comma-separated list of allowed domains"));
    addLineEdit(i18n("Allowed Domains List"), mAllowedDomainsList, QStringLiteral("Accounts_AllowedDomainsList"));

    mBlockedDomainsList->setObjectName(QStringLiteral("mBlockedDomainsList"));
    mBlockedDomainsList->setToolTip(i18n("Comma-separated list of blocked domains"));
    addLineEdit(i18n("Blocked Domains List"), mBlockedDomainsList, QStringLiteral("Accounts_BlockedDomainsList"));

    mBlockedUsernameList->setObjectName(QStringLiteral("mBlockedUsernameList"));
    mBlockedUsernameList->setToolTip(i18n("Comma-separated list of blocked usernames (case-insensitive)"));
    addLineEdit(i18n("Blocked Username List"), mBlockedUsernameList, QStringLiteral("Accounts_BlockedUsernameList"));

    mUseDefaultBlockedDomainsList->setObjectName(QStringLiteral("mUseDefaultBlockedDomainsList"));
    mMainLayout->addWidget(mUseDefaultBlockedDomainsList);
    connectCheckBox(mUseDefaultBlockedDomainsList, QStringLiteral("Accounts_UseDefaultBlockedDomainsList"));

    mUseDNSDomainCheck->setObjectName(QStringLiteral("mUseDNSDomainCheck"));
    mMainLayout->addWidget(mUseDNSDomainCheck);
    connectCheckBox(mUseDNSDomainCheck, QStringLiteral("Accounts_UseDNSDomainCheck"));
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
    initializeWidget(mHowManyFailedAttemptsUntilBlockByIP, mapSettings, 50);
    initializeWidget(mSendEmailUserWhenUserActivated, mapSettings, true);
    initializeWidget(mSendEmailUserWhenUserDeactivated, mapSettings, true);
    initializeWidget(mRequireNameSignup, mapSettings, true);
    initializeWidget(mRequirePasswordConfirmation, mapSettings, true);
    initializeWidget(mOnlyAllowVerifiedUsersLogin, mapSettings, false);
    initializeWidget(mMarkEmailExternalAccountsVerified, mapSettings, true);
    initializeWidget(mManualApproveNewUser, mapSettings, false);
    initializeWidget(mDefaultUsernamePrefixSuggestion, mapSettings, QStringLiteral("user"));
    initializeWidget(mAllowedDomainsList, mapSettings, QString());
    initializeWidget(mBlockedDomainsList, mapSettings, QString());
    initializeWidget(mBlockedUsernameList, mapSettings, QString());
    initializeWidget(mUseDefaultBlockedDomainsList, mapSettings, true);
    initializeWidget(mUseDNSDomainCheck, mapSettings, false);
}
