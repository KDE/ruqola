/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountsettingswidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>

AccountSettingsWidget::AccountSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mAllowChangeName(new QCheckBox(i18nc("@option:check", "Allow Name Change"), this))
    , mAllowCustomStatusMessage(new QCheckBox(i18nc("@option:check", "Allow Custom Status Message"), this))
    , mAllowChangeUserProfile(new QCheckBox(i18nc("@option:check", "Allow User Profile Change"), this))
    , mAllowChangeUserAvatar(new QCheckBox(i18nc("@option:check", "Allow User Avatar Change"), this))
    , mAllowChangeEmail(new QCheckBox(i18nc("@option:check", "Allow Email Change"), this))
    , mAllowChangePassword(new QCheckBox(i18nc("@option:check", "Allow Password Change"), this))
    , mAllowChangeEmailNotifications(new QCheckBox(i18nc("@option:check", "Allow Email Notifications"), this))
    , mAllowUsersToDeleteOwnAccount(new QCheckBox(i18nc("@option:check", "Allow Users to Delete Own Account"), this))
    , mAllowAnonymousWrite(new QCheckBox(i18nc("@option:check", "Allow Anonymous Write"), this))
    , mAllowAnonymousRead(new QCheckBox(i18nc("@option:check", "Allow Anonymous Read"), this))
    , mAllowUsersDeleteOwnAccount(new QCheckBox(i18nc("@option:check", "Allow Users to Delete Own Account"), this))
    , mAllowPasswordChangeOauthUsers(new QCheckBox(i18nc("@option:check", "Allow Password Change for OAuth Users"), this))
    , mAllowFeaturePreview(new QCheckBox(i18nc("@option:check", "Allow Feature Preview"), this))
    , mLoginExpirationInDays(new QSpinBox(this))
    , mAllowInvisibleStatusOption(new QCheckBox(i18nc("@option:check", "Allow Invisible status option"), this))
    , mForgetUserSessionWindowClose(new QCheckBox(i18nc("@option:check", "Forget User Session on Window Close"), this))
    , mEnableCollectLog(new QCheckBox(i18nc("@option:check", "Enable collect log in data"), this))
    , mBlockFailedLoginAttemptsUsername(new QCheckBox(i18nc("@option:check", "Block Failed Login Attempts by Username"), this))
    , mHowManyFailedAttemptsUntilBlockUser(new QSpinBox(this))
    , mTimeUnblockUser(new QSpinBox(this))
    , mHowManyFailedAttemptsUntilBlockByIP(new QSpinBox(this))
    , mHowManyFailedAttemptsUntilBlockByIPinMinutes(new QSpinBox(this))
    , mSendEmailUserWhenUserActivated(new QCheckBox(i18nc("@option:check", "Send Email to User when User is Activated"), this))
    , mSendEmailUserWhenUserDeactivated(new QCheckBox(i18nc("@option:check", "Send Email to User when User is Deactivated"), this))
    , mRequireNameSignup(new QCheckBox(i18nc("@option:check", "Require Name For Signup"), this))
    , mRequirePasswordConfirmation(new QCheckBox(i18nc("@option:check", "Require Password Confirmation"), this))
    , mOnlyAllowVerifiedUsersLogin(new QCheckBox(i18nc("@option:check", "Only Allow Verified Users to Login"), this))
    , mMarkEmailExternalAccountsVerified(new QCheckBox(i18nc("@option:check", "Mark Email for External Accounts Verified"), this))
    , mManualApproveNewUser(new QCheckBox(i18nc("@option:check", "Manually Approve New Users"), this))
    , mDefaultUsernamePrefixSuggestion(new QLineEdit(this))
    , mAllowedDomainsList(new QLineEdit(this))
    , mBlockedDomainsList(new QLineEdit(this))
    , mBlockedUsernameList(new QLineEdit(this))
    , mUseDefaultBlockedDomainsList(new QCheckBox(i18nc("@option:check", "Use Default Blocked Domains List"), this))
    , mUseDNSDomainCheck(new QCheckBox(i18nc("@option:check", "Use DNS Domain Check"), this))
    , mRegistrationAuthenticationServices(new QCheckBox(i18nc("@option:check", "Registration with Authentication Services"), this))
    , mRegistrationAuthenticationServicesDefaultRoles(new QLineEdit(this))
    , mRegistrationUsersDefaultRoles(new QLineEdit(this))
    , mPasswordReset(new QCheckBox(i18nc("@option:check", "Password Reset"), this))
    , mLoginLogs(new QCheckBox(i18nc("@option:check", "Log (on console) failed login attempts"), this))
    , mLoginLogsUsername(new QCheckBox(i18nc("@option:check", "Show Username on failed login attempts logs"), this))
    , mLoginLogsUseragent(new QCheckBox(i18nc("@option:check", "Show UserAgent on failed login attempts logs"), this))
    , mLoginLogsClientIp(new QCheckBox(i18nc("@option:check", "Show Client IP on failed login attempts logs"), this))
    , mLoginLogsForwardedForIp(new QCheckBox(i18nc("@option:check", "Show Forwarded IP on failed login attempts logs"), this))
{
    mAllowChangeName->setObjectName(u"mAllowChangeName"_s);
    addCheckBox(mAllowChangeName, u"Accounts_AllowRealNameChange"_s);

    mAllowCustomStatusMessage->setObjectName(u"mAllowCustomStatusMessage"_s);
    addCheckBox(mAllowCustomStatusMessage, u"Accounts_AllowUserStatusMessageChange"_s);

    mAllowChangeUserProfile->setObjectName(u"mAllowChangeUserProfile"_s);
    addCheckBox(mAllowChangeUserProfile, u"Accounts_AllowUserProfileChange"_s);

    mAllowChangeUserAvatar->setObjectName(u"mAllowChangeUserAvatar"_s);
    addCheckBox(mAllowChangeUserAvatar, u"Accounts_AllowUserAvatarChange"_s);

    mAllowChangeEmail->setObjectName(u"mAllowChangeEmail"_s);
    addCheckBox(mAllowChangeEmail, u"Accounts_AllowEmailChange"_s);

    mAllowChangePassword->setObjectName(u"mAllowChangePassword"_s);
    addCheckBox(mAllowChangePassword, u"Accounts_AllowPasswordChange"_s);

    mAllowChangeEmailNotifications->setObjectName(u"mAllowChangeEmailNotifications"_s);
    addCheckBox(mAllowChangeEmailNotifications, u"Accounts_AllowEmailNotifications"_s);

    mAllowUsersToDeleteOwnAccount->setObjectName(u"mAllowUsersToDeleteOwnAccount"_s);
    addCheckBox(mAllowUsersToDeleteOwnAccount, u"Accounts_AllowDeleteOwnAccount"_s);

    mAllowAnonymousWrite->setObjectName(u"mAllowAnonymousWrite"_s);
    addCheckBox(mAllowAnonymousWrite, u"Accounts_AllowAnonymousWrite"_s);

    mAllowAnonymousRead->setObjectName(u"mAllowAnonymousRead"_s);
    addCheckBox(mAllowAnonymousRead, u"Accounts_AllowAnonymousRead"_s);

    mAllowUsersDeleteOwnAccount->setObjectName(u"mAllowUsersDeleteOwnAccount"_s);
    addCheckBox(mAllowUsersDeleteOwnAccount, u"Accounts_AllowDeleteOwnAccount"_s);

    mAllowPasswordChangeOauthUsers->setObjectName(u"mAllowPasswordChangeOauthUsers"_s);
    addCheckBox(mAllowPasswordChangeOauthUsers, u"Accounts_AllowPasswordChangeForOAuthUsers"_s);

    mAllowFeaturePreview->setObjectName(u"mAllowFeaturePreview"_s);
    addCheckBox(mAllowFeaturePreview, u"Accounts_AllowFeaturePreview"_s);

    mLoginExpirationInDays->setObjectName(u"mLoginExpirationInDays"_s);
    addSpinbox(i18n("Login Expiration in Days"), mLoginExpirationInDays, u"Accounts_LoginExpiration"_s);

    mAllowInvisibleStatusOption->setObjectName(u"mAllowInvisibleStatusOption"_s);
    addCheckBox(mAllowInvisibleStatusOption, u"Accounts_AllowInvisibleStatusOption"_s);

    mForgetUserSessionWindowClose->setObjectName(u"mForgetUserSessionWindowClose"_s);
    addCheckBox(mForgetUserSessionWindowClose, u"Accounts_ForgetUserSessionOnWindowClose"_s);

    auto failedLoginAttemptsLabel = createBoldLabel(i18n("Failed Login Attempts"));
    failedLoginAttemptsLabel->setObjectName(u"failedLoginAttemptsLabel"_s);
    mMainLayout->addWidget(failedLoginAttemptsLabel);

    mEnableCollectLog->setObjectName(u"mEnableCollectLog"_s);
    addCheckBox(mEnableCollectLog, u"Block_Multiple_Failed_Logins_Enabled"_s);

    mBlockFailedLoginAttemptsUsername->setObjectName(u"mBlockFailedLoginAttemptsUsername"_s);
    addCheckBox(mBlockFailedLoginAttemptsUsername, u"Block_Multiple_Failed_Logins_By_User"_s);

    mHowManyFailedAttemptsUntilBlockUser->setObjectName(u"mHowManyFailedAttemptsUntilBlockUser"_s);
    addSpinbox(i18n("How Many Failed Attempts Until Block by User"),
               mHowManyFailedAttemptsUntilBlockUser,
               u"Block_Multiple_Failed_Logins_Attempts_Until_Block_by_User"_s);

    mTimeUnblockUser->setObjectName(u"mTimeUnblockUser"_s);
    addSpinbox(i18n("Time to unblock User (In Minutes)"), mTimeUnblockUser, u"Block_Multiple_Failed_Logins_Time_To_Unblock_By_User_In_Minutes"_s);

    mHowManyFailedAttemptsUntilBlockByIP->setObjectName(u"mHowManyFailedAttemptsUntilBlockByIP"_s);
    addSpinbox(i18n("How many failed attempts until block by IP"),
               mHowManyFailedAttemptsUntilBlockByIP,
               u"Block_Multiple_Failed_Logins_Attempts_Until_Block_By_Ip"_s);

    // TODO add missing settings
    mHowManyFailedAttemptsUntilBlockByIPinMinutes->setObjectName(u"mHowManyFailedAttemptsUntilBlockByIPinMinutes"_s);
    addSpinbox(i18n("How many failed attempts until block by IP"),
               mHowManyFailedAttemptsUntilBlockByIPinMinutes,
               u"Block_Multiple_Failed_Logins_Time_To_Unblock_By_Ip_In_Minutes"_s);

    auto loginLogsLabel = createBoldLabel(i18n("Login Logs"));
    loginLogsLabel->setObjectName(u"loginLogsLabel"_s);
    mMainLayout->addWidget(loginLogsLabel);

    mLoginLogs->setObjectName(u"mLoginLogs"_s);
    addCheckBox(mLoginLogs, u"Login_Logs_Enabled"_s);

    mLoginLogsUsername->setObjectName(u"mLoginLogsUsername"_s);
    addCheckBox(mLoginLogsUsername, u"Login_Logs_Username"_s);

    mLoginLogsUseragent->setObjectName(u"mLoginLogsUseragent"_s);
    addCheckBox(mLoginLogsUseragent, u"Login_Logs_UserAgent"_s);

    mLoginLogsClientIp->setObjectName(u"mLoginLogsClientIp"_s);
    addCheckBox(mLoginLogsClientIp, u"Login_Logs_ClientIp"_s);

    mLoginLogsForwardedForIp->setObjectName(u"mLoginLogsForwardedForIp"_s);
    addCheckBox(mLoginLogsForwardedForIp, u"mLoginLogsForwardedForIp"_s);

    auto regislationLabel = createBoldLabel(i18n("Registration"));
    regislationLabel->setObjectName(u"regislationLabel"_s);
    mMainLayout->addWidget(regislationLabel);

    mSendEmailUserWhenUserActivated->setObjectName(u"mSendEmailUserWhenUserActivated"_s);
    addCheckBox(mSendEmailUserWhenUserActivated, u"Accounts_Send_Email_When_Activating"_s);

    mSendEmailUserWhenUserDeactivated->setObjectName(u"mSendEmailUserWhenUserDeactivated"_s);
    addCheckBox(mSendEmailUserWhenUserDeactivated, u"Accounts_Send_Email_When_Deactivating"_s);

    mRequireNameSignup->setObjectName(u"mRequireNameSignup"_s);
    addCheckBox(mRequireNameSignup, u"Accounts_RequireNameForSignUp"_s);

    mRequirePasswordConfirmation->setObjectName(u"mRequirePasswordConfirmation"_s);
    addCheckBox(mRequirePasswordConfirmation, u"Accounts_RequirePasswordConfirmation"_s);

    mOnlyAllowVerifiedUsersLogin->setObjectName(u"mOnlyAllowVerifiedUsersLogin"_s);
    mOnlyAllowVerifiedUsersLogin->setToolTip(i18nc("@info:tooltip", "Make sure you have correct SMTP settings to use this feature"));
    addCheckBox(mOnlyAllowVerifiedUsersLogin, u"Accounts_EmailVerification"_s);

    mMarkEmailExternalAccountsVerified->setObjectName(u"mMarkEmailExternalAccountsVerified"_s);
    addCheckBox(mMarkEmailExternalAccountsVerified, u"Accounts_Verify_Email_For_External_Accounts"_s);

    mManualApproveNewUser->setObjectName(u"mManualApproveNewUser"_s);
    addCheckBox(mManualApproveNewUser, u"Accounts_ManuallyApproveNewUsers"_s);

    mDefaultUsernamePrefixSuggestion->setObjectName(u"mDefaultUsernamePrefixSuggestion"_s);
    addLineEdit(i18n("Default Username Prefix Suggestion"), mDefaultUsernamePrefixSuggestion, u"Accounts_DefaultUsernamePrefixSuggestion"_s);

    mAllowedDomainsList->setObjectName(u"mAllowedDomainsList"_s);
    mAllowedDomainsList->setToolTip(i18nc("@info:tooltip", "Comma-separated list of allowed domains"));
    addLineEdit(i18n("Allowed Domains List"), mAllowedDomainsList, u"Accounts_AllowedDomainsList"_s);

    mBlockedDomainsList->setObjectName(u"mBlockedDomainsList"_s);
    mBlockedDomainsList->setToolTip(i18nc("@info:tooltip", "Comma-separated list of blocked domains"));
    addLineEdit(i18n("Blocked Domains List"), mBlockedDomainsList, u"Accounts_BlockedDomainsList"_s);

    mBlockedUsernameList->setObjectName(u"mBlockedUsernameList"_s);
    mBlockedUsernameList->setToolTip(i18nc("@info:tooltip", "Comma-separated list of blocked usernames (case-insensitive)"));
    addLineEdit(i18n("Blocked Username List"), mBlockedUsernameList, u"Accounts_BlockedUsernameList"_s);

    mUseDefaultBlockedDomainsList->setObjectName(u"mUseDefaultBlockedDomainsList"_s);
    addCheckBox(mUseDefaultBlockedDomainsList, u"Accounts_UseDefaultBlockedDomainsList"_s);

    mUseDNSDomainCheck->setObjectName(u"mUseDNSDomainCheck"_s);
    addCheckBox(mUseDNSDomainCheck, u"Accounts_UseDNSDomainCheck"_s);

    mRegistrationAuthenticationServices->setObjectName(u"mRegistrationAuthenticationServices"_s);
    mMainLayout->addWidget(mRegistrationAuthenticationServices);
    addCheckBox(mRegistrationAuthenticationServices, u"Accounts_Registration_AuthenticationServices_Enabled"_s);

    mRegistrationAuthenticationServicesDefaultRoles->setObjectName(u"mRegistrationAuthenticationServicesDefaultRoles"_s);
    mRegistrationAuthenticationServicesDefaultRoles->setToolTip(
        i18n("Default roles (comma-separated) users will be given when registering through authentication services"));
    addLineEdit(i18n("Default Roles for Authentication Services"),
                mRegistrationAuthenticationServicesDefaultRoles,
                u"Accounts_Registration_AuthenticationServices_Default_Roles"_s);

    mRegistrationUsersDefaultRoles->setObjectName(u"mRegistrationUsersDefaultRoles"_s);
    mRegistrationUsersDefaultRoles->setToolTip(
        i18n("Default roles (comma-separated) users will be given when registering through manual registration (including via API)"));
    addLineEdit(i18n("Default Roles for Users"), mRegistrationUsersDefaultRoles, u"Accounts_Registration_Users_Default_Roles"_s);

    mPasswordReset->setObjectName(u"mPasswordReset"_s);
    addCheckBox(mPasswordReset, u"Accounts_PasswordReset"_s);
}

AccountSettingsWidget::~AccountSettingsWidget() = default;

void AccountSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    // qDebug() << " mapSettings " << mapSettings;
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
    initializeWidget(mHowManyFailedAttemptsUntilBlockByIPinMinutes, mapSettings, 5);
    initializeWidget(mSendEmailUserWhenUserActivated, mapSettings, true);
    initializeWidget(mSendEmailUserWhenUserDeactivated, mapSettings, true);
    initializeWidget(mRequireNameSignup, mapSettings, true);
    initializeWidget(mRequirePasswordConfirmation, mapSettings, true);
    initializeWidget(mOnlyAllowVerifiedUsersLogin, mapSettings, false);
    initializeWidget(mMarkEmailExternalAccountsVerified, mapSettings, true);
    initializeWidget(mManualApproveNewUser, mapSettings, false);
    initializeWidget(mDefaultUsernamePrefixSuggestion, mapSettings, u"user"_s);
    initializeWidget(mAllowedDomainsList, mapSettings, QString());
    initializeWidget(mBlockedDomainsList, mapSettings, QString());
    initializeWidget(mBlockedUsernameList, mapSettings, QString());
    initializeWidget(mUseDefaultBlockedDomainsList, mapSettings, true);
    initializeWidget(mUseDNSDomainCheck, mapSettings, false);
    initializeWidget(mAllowFeaturePreview, mapSettings, false);
    initializeWidget(mRegistrationAuthenticationServices, mapSettings, true);
    initializeWidget(mRegistrationAuthenticationServicesDefaultRoles, mapSettings, u"user"_s);
    initializeWidget(mRegistrationUsersDefaultRoles, mapSettings, u"user"_s);
    initializeWidget(mPasswordReset, mapSettings, true);
    initializeWidget(mLoginLogs, mapSettings, true);
    initializeWidget(mLoginLogsUsername, mapSettings, false);
    initializeWidget(mLoginLogsUseragent, mapSettings, false);
    initializeWidget(mLoginLogsClientIp, mapSettings, false);
    initializeWidget(mLoginLogsForwardedForIp, mapSettings, false);
}

#include "moc_accountsettingswidget.cpp"
