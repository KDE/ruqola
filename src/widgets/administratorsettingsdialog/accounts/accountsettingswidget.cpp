/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

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
    mAllowChangeName->setObjectName(QStringLiteral("mAllowChangeName"));
    addCheckBox(mAllowChangeName, QStringLiteral("Accounts_AllowRealNameChange"));

    mAllowCustomStatusMessage->setObjectName(QStringLiteral("mAllowCustomStatusMessage"));
    addCheckBox(mAllowCustomStatusMessage, QStringLiteral("Accounts_AllowUserStatusMessageChange"));

    mAllowChangeUserProfile->setObjectName(QStringLiteral("mAllowChangeUserProfile"));
    addCheckBox(mAllowChangeUserProfile, QStringLiteral("Accounts_AllowUserProfileChange"));

    mAllowChangeUserAvatar->setObjectName(QStringLiteral("mAllowChangeUserAvatar"));
    addCheckBox(mAllowChangeUserAvatar, QStringLiteral("Accounts_AllowUserAvatarChange"));

    mAllowChangeEmail->setObjectName(QStringLiteral("mAllowChangeEmail"));
    addCheckBox(mAllowChangeEmail, QStringLiteral("Accounts_AllowEmailChange"));

    mAllowChangePassword->setObjectName(QStringLiteral("mAllowChangePassword"));
    addCheckBox(mAllowChangePassword, QStringLiteral("Accounts_AllowPasswordChange"));

    mAllowChangeEmailNotifications->setObjectName(QStringLiteral("mAllowChangeEmailNotifications"));
    addCheckBox(mAllowChangeEmailNotifications, QStringLiteral("Accounts_AllowEmailNotifications"));

    mAllowUsersToDeleteOwnAccount->setObjectName(QStringLiteral("mAllowUsersToDeleteOwnAccount"));
    addCheckBox(mAllowUsersToDeleteOwnAccount, QStringLiteral("Accounts_AllowDeleteOwnAccount"));

    mAllowAnonymousWrite->setObjectName(QStringLiteral("mAllowAnonymousWrite"));
    addCheckBox(mAllowAnonymousWrite, QStringLiteral("Accounts_AllowAnonymousWrite"));

    mAllowAnonymousRead->setObjectName(QStringLiteral("mAllowAnonymousRead"));
    addCheckBox(mAllowAnonymousRead, QStringLiteral("Accounts_AllowAnonymousRead"));

    mAllowUsersDeleteOwnAccount->setObjectName(QStringLiteral("mAllowUsersDeleteOwnAccount"));
    addCheckBox(mAllowUsersDeleteOwnAccount, QStringLiteral("Accounts_AllowDeleteOwnAccount"));

    mAllowPasswordChangeOauthUsers->setObjectName(QStringLiteral("mAllowPasswordChangeOauthUsers"));
    addCheckBox(mAllowPasswordChangeOauthUsers, QStringLiteral("Accounts_AllowPasswordChangeForOAuthUsers"));

    mAllowFeaturePreview->setObjectName(QStringLiteral("mAllowFeaturePreview"));
    addCheckBox(mAllowFeaturePreview, QStringLiteral("Accounts_AllowFeaturePreview"));

    mLoginExpirationInDays->setObjectName(QStringLiteral("mLoginExpirationInDays"));
    addSpinbox(i18n("Login Expiration in Days"), mLoginExpirationInDays, QStringLiteral("Accounts_LoginExpiration"));

    mAllowInvisibleStatusOption->setObjectName(QStringLiteral("mAllowInvisibleStatusOption"));
    addCheckBox(mAllowInvisibleStatusOption, QStringLiteral("Accounts_AllowInvisibleStatusOption"));

    mForgetUserSessionWindowClose->setObjectName(QStringLiteral("mForgetUserSessionWindowClose"));
    addCheckBox(mForgetUserSessionWindowClose, QStringLiteral("Accounts_ForgetUserSessionOnWindowClose"));

    auto failedLoginAttemptsLabel = createBoldLabel(i18n("Failed Login Attempts"));
    failedLoginAttemptsLabel->setObjectName(QStringLiteral("failedLoginAttemptsLabel"));
    mMainLayout->addWidget(failedLoginAttemptsLabel);

    mEnableCollectLog->setObjectName(QStringLiteral("mEnableCollectLog"));
    addCheckBox(mEnableCollectLog, QStringLiteral("Block_Multiple_Failed_Logins_Enabled"));

    mBlockFailedLoginAttemptsUsername->setObjectName(QStringLiteral("mBlockFailedLoginAttemptsUsername"));
    addCheckBox(mBlockFailedLoginAttemptsUsername, QStringLiteral("Block_Multiple_Failed_Logins_By_User"));

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

    // TODO add missing settings
    mHowManyFailedAttemptsUntilBlockByIPinMinutes->setObjectName(QStringLiteral("mHowManyFailedAttemptsUntilBlockByIPinMinutes"));
    addSpinbox(i18n("How many failed attempts until block by IP"),
               mHowManyFailedAttemptsUntilBlockByIPinMinutes,
               QStringLiteral("Block_Multiple_Failed_Logins_Time_To_Unblock_By_Ip_In_Minutes"));

    auto loginLogsLabel = createBoldLabel(i18n("Login Logs"));
    loginLogsLabel->setObjectName(QStringLiteral("loginLogsLabel"));
    mMainLayout->addWidget(loginLogsLabel);

    mLoginLogs->setObjectName(QStringLiteral("mLoginLogs"));
    addCheckBox(mLoginLogs, QStringLiteral("Login_Logs_Enabled"));

    mLoginLogsUsername->setObjectName(QStringLiteral("mLoginLogsUsername"));
    addCheckBox(mLoginLogsUsername, QStringLiteral("Login_Logs_Username"));

    mLoginLogsUseragent->setObjectName(QStringLiteral("mLoginLogsUseragent"));
    addCheckBox(mLoginLogsUseragent, QStringLiteral("Login_Logs_UserAgent"));

    mLoginLogsClientIp->setObjectName(QStringLiteral("mLoginLogsClientIp"));
    addCheckBox(mLoginLogsClientIp, QStringLiteral("Login_Logs_ClientIp"));

    mLoginLogsForwardedForIp->setObjectName(QStringLiteral("mLoginLogsForwardedForIp"));
    addCheckBox(mLoginLogsForwardedForIp, QStringLiteral("mLoginLogsForwardedForIp"));

    auto regislationLabel = createBoldLabel(i18n("Registration"));
    regislationLabel->setObjectName(QStringLiteral("regislationLabel"));
    mMainLayout->addWidget(regislationLabel);

    mSendEmailUserWhenUserActivated->setObjectName(QStringLiteral("mSendEmailUserWhenUserActivated"));
    addCheckBox(mSendEmailUserWhenUserActivated, QStringLiteral("Accounts_Send_Email_When_Activating"));

    mSendEmailUserWhenUserDeactivated->setObjectName(QStringLiteral("mSendEmailUserWhenUserDeactivated"));
    addCheckBox(mSendEmailUserWhenUserDeactivated, QStringLiteral("Accounts_Send_Email_When_Deactivating"));

    mRequireNameSignup->setObjectName(QStringLiteral("mRequireNameSignup"));
    addCheckBox(mRequireNameSignup, QStringLiteral("Accounts_RequireNameForSignUp"));

    mRequirePasswordConfirmation->setObjectName(QStringLiteral("mRequirePasswordConfirmation"));
    addCheckBox(mRequirePasswordConfirmation, QStringLiteral("Accounts_RequirePasswordConfirmation"));

    mOnlyAllowVerifiedUsersLogin->setObjectName(QStringLiteral("mOnlyAllowVerifiedUsersLogin"));
    mOnlyAllowVerifiedUsersLogin->setToolTip(i18nc("@info:tooltip", "Make sure you have correct SMTP settings to use this feature"));
    addCheckBox(mOnlyAllowVerifiedUsersLogin, QStringLiteral("Accounts_EmailVerification"));

    mMarkEmailExternalAccountsVerified->setObjectName(QStringLiteral("mMarkEmailExternalAccountsVerified"));
    addCheckBox(mMarkEmailExternalAccountsVerified, QStringLiteral("Accounts_Verify_Email_For_External_Accounts"));

    mManualApproveNewUser->setObjectName(QStringLiteral("mManualApproveNewUser"));
    addCheckBox(mManualApproveNewUser, QStringLiteral("Accounts_ManuallyApproveNewUsers"));

    mDefaultUsernamePrefixSuggestion->setObjectName(QStringLiteral("mDefaultUsernamePrefixSuggestion"));
    addLineEdit(i18n("Default Username Prefix Suggestion"), mDefaultUsernamePrefixSuggestion, QStringLiteral("Accounts_DefaultUsernamePrefixSuggestion"));

    mAllowedDomainsList->setObjectName(QStringLiteral("mAllowedDomainsList"));
    mAllowedDomainsList->setToolTip(i18nc("@info:tooltip", "Comma-separated list of allowed domains"));
    addLineEdit(i18n("Allowed Domains List"), mAllowedDomainsList, QStringLiteral("Accounts_AllowedDomainsList"));

    mBlockedDomainsList->setObjectName(QStringLiteral("mBlockedDomainsList"));
    mBlockedDomainsList->setToolTip(i18nc("@info:tooltip", "Comma-separated list of blocked domains"));
    addLineEdit(i18n("Blocked Domains List"), mBlockedDomainsList, QStringLiteral("Accounts_BlockedDomainsList"));

    mBlockedUsernameList->setObjectName(QStringLiteral("mBlockedUsernameList"));
    mBlockedUsernameList->setToolTip(i18nc("@info:tooltip", "Comma-separated list of blocked usernames (case-insensitive)"));
    addLineEdit(i18n("Blocked Username List"), mBlockedUsernameList, QStringLiteral("Accounts_BlockedUsernameList"));

    mUseDefaultBlockedDomainsList->setObjectName(QStringLiteral("mUseDefaultBlockedDomainsList"));
    addCheckBox(mUseDefaultBlockedDomainsList, QStringLiteral("Accounts_UseDefaultBlockedDomainsList"));

    mUseDNSDomainCheck->setObjectName(QStringLiteral("mUseDNSDomainCheck"));
    addCheckBox(mUseDNSDomainCheck, QStringLiteral("Accounts_UseDNSDomainCheck"));

    mRegistrationAuthenticationServices->setObjectName(QStringLiteral("mRegistrationAuthenticationServices"));
    mMainLayout->addWidget(mRegistrationAuthenticationServices);
    addCheckBox(mRegistrationAuthenticationServices, QStringLiteral("Accounts_Registration_AuthenticationServices_Enabled"));

    mRegistrationAuthenticationServicesDefaultRoles->setObjectName(QStringLiteral("mRegistrationAuthenticationServicesDefaultRoles"));
    mRegistrationAuthenticationServicesDefaultRoles->setToolTip(
        i18n("Default roles (comma-separated) users will be given when registering through authentication services"));
    addLineEdit(i18n("Default Roles for Authentication Services"),
                mRegistrationAuthenticationServicesDefaultRoles,
                QStringLiteral("Accounts_Registration_AuthenticationServices_Default_Roles"));

    mRegistrationUsersDefaultRoles->setObjectName(QStringLiteral("mRegistrationUsersDefaultRoles"));
    mRegistrationUsersDefaultRoles->setToolTip(
        i18n("Default roles (comma-separated) users will be given when registering through manual registration (including via API)"));
    addLineEdit(i18n("Default Roles for Users"), mRegistrationUsersDefaultRoles, QStringLiteral("Accounts_Registration_Users_Default_Roles"));

    mPasswordReset->setObjectName(QStringLiteral("mPasswordReset"));
    addCheckBox(mPasswordReset, QStringLiteral("Accounts_PasswordReset"));
}

AccountSettingsWidget::~AccountSettingsWidget() = default;

void AccountSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
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
    initializeWidget(mDefaultUsernamePrefixSuggestion, mapSettings, QStringLiteral("user"));
    initializeWidget(mAllowedDomainsList, mapSettings, QString());
    initializeWidget(mBlockedDomainsList, mapSettings, QString());
    initializeWidget(mBlockedUsernameList, mapSettings, QString());
    initializeWidget(mUseDefaultBlockedDomainsList, mapSettings, true);
    initializeWidget(mUseDNSDomainCheck, mapSettings, false);
    initializeWidget(mAllowFeaturePreview, mapSettings, false);
    initializeWidget(mRegistrationAuthenticationServices, mapSettings, true);
    initializeWidget(mRegistrationAuthenticationServicesDefaultRoles, mapSettings, QStringLiteral("user"));
    initializeWidget(mRegistrationUsersDefaultRoles, mapSettings, QStringLiteral("user"));
    initializeWidget(mPasswordReset, mapSettings, true);
    initializeWidget(mLoginLogs, mapSettings, true);
    initializeWidget(mLoginLogsUsername, mapSettings, false);
    initializeWidget(mLoginLogsUseragent, mapSettings, false);
    initializeWidget(mLoginLogsClientIp, mapSettings, false);
    initializeWidget(mLoginLogsForwardedForIp, mapSettings, false);
}

#include "moc_accountsettingswidget.cpp"
