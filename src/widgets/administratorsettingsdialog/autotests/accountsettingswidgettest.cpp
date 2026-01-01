/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountsettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/accounts/accountsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>

QTEST_MAIN(AccountSettingsWidgetTest)

AccountSettingsWidgetTest::AccountSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AccountSettingsWidgetTest::shouldHaveDefaultValues()
{
    AccountSettingsWidget w(nullptr);
    QVERIFY(w.widgetResizable());

    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mAllowChangeName = w.findChild<QCheckBox *>(u"mAllowChangeName"_s);
    QVERIFY(mAllowChangeName);
    QVERIFY(!mAllowChangeName->isChecked());
    QVERIFY(!mAllowChangeName->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowChangeName), u"Accounts_AllowRealNameChange"_s);

    auto mAllowChangeUserProfile = w.findChild<QCheckBox *>(u"mAllowChangeUserProfile"_s);
    QVERIFY(mAllowChangeUserProfile);
    QVERIFY(!mAllowChangeUserProfile->isChecked());
    QVERIFY(!mAllowChangeUserProfile->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowChangeUserProfile), u"Accounts_AllowUserProfileChange"_s);

    auto mAllowChangeUserAvatar = w.findChild<QCheckBox *>(u"mAllowChangeUserAvatar"_s);
    QVERIFY(mAllowChangeUserAvatar);
    QVERIFY(!mAllowChangeUserAvatar->isChecked());
    QVERIFY(!mAllowChangeUserAvatar->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowChangeUserAvatar), u"Accounts_AllowUserAvatarChange"_s);

    auto mAllowChangeEmail = w.findChild<QCheckBox *>(u"mAllowChangeEmail"_s);
    QVERIFY(mAllowChangeEmail);
    QVERIFY(!mAllowChangeEmail->isChecked());
    QVERIFY(!mAllowChangeEmail->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowChangeEmail), u"Accounts_AllowEmailChange"_s);

    auto mAllowChangePassword = w.findChild<QCheckBox *>(u"mAllowChangePassword"_s);
    QVERIFY(mAllowChangePassword);
    QVERIFY(!mAllowChangePassword->isChecked());
    QVERIFY(!mAllowChangePassword->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowChangePassword), u"Accounts_AllowPasswordChange"_s);

    auto mAllowChangeEmailNotifications = w.findChild<QCheckBox *>(u"mAllowChangeEmailNotifications"_s);
    QVERIFY(mAllowChangeEmailNotifications);
    QVERIFY(!mAllowChangeEmailNotifications->isChecked());
    QVERIFY(!mAllowChangeEmailNotifications->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowChangeEmailNotifications), u"Accounts_AllowEmailNotifications"_s);

    auto mAllowUsersToDeleteOwnAccount = w.findChild<QCheckBox *>(u"mAllowUsersToDeleteOwnAccount"_s);
    QVERIFY(mAllowUsersToDeleteOwnAccount);
    QVERIFY(!mAllowUsersToDeleteOwnAccount->isChecked());
    QVERIFY(!mAllowUsersToDeleteOwnAccount->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowUsersToDeleteOwnAccount), u"Accounts_AllowDeleteOwnAccount"_s);

    auto mAllowAnonymousWrite = w.findChild<QCheckBox *>(u"mAllowAnonymousWrite"_s);
    QVERIFY(mAllowAnonymousWrite);
    QVERIFY(!mAllowAnonymousWrite->isChecked());
    QVERIFY(!mAllowAnonymousWrite->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowAnonymousWrite), u"Accounts_AllowAnonymousWrite"_s);

    auto mAllowAnonymousRead = w.findChild<QCheckBox *>(u"mAllowAnonymousRead"_s);
    QVERIFY(mAllowAnonymousRead);
    QVERIFY(!mAllowAnonymousRead->isChecked());
    QVERIFY(!mAllowAnonymousRead->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowAnonymousRead), u"Accounts_AllowAnonymousRead"_s);

    auto mAllowUsersDeleteOwnAccount = w.findChild<QCheckBox *>(u"mAllowUsersDeleteOwnAccount"_s);
    QVERIFY(mAllowUsersDeleteOwnAccount);
    QVERIFY(!mAllowUsersDeleteOwnAccount->isChecked());
    QVERIFY(!mAllowUsersDeleteOwnAccount->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowUsersDeleteOwnAccount), u"Accounts_AllowDeleteOwnAccount"_s);

    auto mAllowPasswordChangeOauthUsers = w.findChild<QCheckBox *>(u"mAllowPasswordChangeOauthUsers"_s);
    QVERIFY(mAllowPasswordChangeOauthUsers);
    QVERIFY(!mAllowPasswordChangeOauthUsers->isChecked());
    QVERIFY(!mAllowPasswordChangeOauthUsers->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowPasswordChangeOauthUsers), u"Accounts_AllowPasswordChangeForOAuthUsers"_s);

    auto mLoginExpirationInDays = w.findChild<QSpinBox *>(u"mLoginExpirationInDays"_s);
    QVERIFY(mLoginExpirationInDays);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLoginExpirationInDays), u"Accounts_LoginExpiration"_s);

    auto mAllowInvisibleStatusOption = w.findChild<QCheckBox *>(u"mAllowInvisibleStatusOption"_s);
    QVERIFY(mAllowInvisibleStatusOption);
    QVERIFY(!mAllowInvisibleStatusOption->isChecked());
    QVERIFY(!mAllowInvisibleStatusOption->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowInvisibleStatusOption), u"Accounts_AllowInvisibleStatusOption"_s);

    auto mForgetUserSessionWindowClose = w.findChild<QCheckBox *>(u"mForgetUserSessionWindowClose"_s);
    QVERIFY(mForgetUserSessionWindowClose);
    QVERIFY(!mForgetUserSessionWindowClose->isChecked());
    QVERIFY(!mForgetUserSessionWindowClose->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mForgetUserSessionWindowClose), u"Accounts_ForgetUserSessionOnWindowClose"_s);

    auto failedLoginAttemptsLabel = w.findChild<QLabel *>(u"failedLoginAttemptsLabel"_s);
    QVERIFY(failedLoginAttemptsLabel);
    QVERIFY(!failedLoginAttemptsLabel->text().isEmpty());

    auto mEnableCollectLog = w.findChild<QCheckBox *>(u"mEnableCollectLog"_s);
    QVERIFY(mEnableCollectLog);
    QVERIFY(!mEnableCollectLog->isChecked());
    QVERIFY(!mEnableCollectLog->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableCollectLog), u"Block_Multiple_Failed_Logins_Enabled"_s);

    auto mAllowCustomStatusMessage = w.findChild<QCheckBox *>(u"mAllowCustomStatusMessage"_s);
    QVERIFY(mAllowCustomStatusMessage);
    QVERIFY(!mAllowCustomStatusMessage->isChecked());
    QVERIFY(!mAllowCustomStatusMessage->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowCustomStatusMessage), u"Accounts_AllowUserStatusMessageChange"_s);

    auto mBlockFailedLoginAttemptsUsername = w.findChild<QCheckBox *>(u"mBlockFailedLoginAttemptsUsername"_s);
    QVERIFY(mBlockFailedLoginAttemptsUsername);
    QVERIFY(!mBlockFailedLoginAttemptsUsername->isChecked());
    QVERIFY(!mBlockFailedLoginAttemptsUsername->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mBlockFailedLoginAttemptsUsername), u"Block_Multiple_Failed_Logins_By_User"_s);

    auto mHowManyFailedAttemptsUntilBlockUser = w.findChild<QSpinBox *>(u"mHowManyFailedAttemptsUntilBlockUser"_s);
    QVERIFY(mHowManyFailedAttemptsUntilBlockUser);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mHowManyFailedAttemptsUntilBlockUser), u"Block_Multiple_Failed_Logins_Attempts_Until_Block_by_User"_s);

    auto mTimeUnblockUser = w.findChild<QSpinBox *>(u"mTimeUnblockUser"_s);
    QVERIFY(mTimeUnblockUser);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTimeUnblockUser), u"Block_Multiple_Failed_Logins_Time_To_Unblock_By_User_In_Minutes"_s);

    auto mHowManyFailedAttemptsUntilBlockByIP = w.findChild<QSpinBox *>(u"mHowManyFailedAttemptsUntilBlockByIP"_s);
    QVERIFY(mHowManyFailedAttemptsUntilBlockByIP);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mHowManyFailedAttemptsUntilBlockByIP), u"Block_Multiple_Failed_Logins_Attempts_Until_Block_By_Ip"_s);

    auto mHowManyFailedAttemptsUntilBlockByIPinMinutes = w.findChild<QSpinBox *>(u"mHowManyFailedAttemptsUntilBlockByIPinMinutes"_s);
    QVERIFY(mHowManyFailedAttemptsUntilBlockByIPinMinutes);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mHowManyFailedAttemptsUntilBlockByIPinMinutes),
             u"Block_Multiple_Failed_Logins_Time_To_Unblock_By_Ip_In_Minutes"_s);

    auto mSendEmailUserWhenUserActivated = w.findChild<QCheckBox *>(u"mSendEmailUserWhenUserActivated"_s);
    QVERIFY(mSendEmailUserWhenUserActivated);
    QVERIFY(!mSendEmailUserWhenUserActivated->isChecked());
    QVERIFY(!mSendEmailUserWhenUserActivated->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSendEmailUserWhenUserActivated), u"Accounts_Send_Email_When_Activating"_s);

    auto mSendEmailUserWhenUserDeactivated = w.findChild<QCheckBox *>(u"mSendEmailUserWhenUserDeactivated"_s);
    QVERIFY(mSendEmailUserWhenUserDeactivated);
    QVERIFY(!mSendEmailUserWhenUserDeactivated->isChecked());
    QVERIFY(!mSendEmailUserWhenUserDeactivated->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSendEmailUserWhenUserDeactivated), u"Accounts_Send_Email_When_Deactivating"_s);

    auto mRequireNameSignup = w.findChild<QCheckBox *>(u"mRequireNameSignup"_s);
    QVERIFY(mRequireNameSignup);
    QVERIFY(!mRequireNameSignup->isChecked());
    QVERIFY(!mRequireNameSignup->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mRequireNameSignup), u"Accounts_RequireNameForSignUp"_s);

    auto mRequirePasswordConfirmation = w.findChild<QCheckBox *>(u"mRequirePasswordConfirmation"_s);
    QVERIFY(mRequirePasswordConfirmation);
    QVERIFY(!mRequirePasswordConfirmation->isChecked());
    QVERIFY(!mRequirePasswordConfirmation->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mRequirePasswordConfirmation), u"Accounts_RequirePasswordConfirmation"_s);

    auto mOnlyAllowVerifiedUsersLogin = w.findChild<QCheckBox *>(u"mOnlyAllowVerifiedUsersLogin"_s);
    QVERIFY(mOnlyAllowVerifiedUsersLogin);
    QVERIFY(!mOnlyAllowVerifiedUsersLogin->isChecked());
    QVERIFY(!mOnlyAllowVerifiedUsersLogin->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mOnlyAllowVerifiedUsersLogin), u"Accounts_EmailVerification"_s);

    auto mMarkEmailExternalAccountsVerified = w.findChild<QCheckBox *>(u"mMarkEmailExternalAccountsVerified"_s);
    QVERIFY(mMarkEmailExternalAccountsVerified);
    QVERIFY(!mMarkEmailExternalAccountsVerified->isChecked());
    QVERIFY(!mMarkEmailExternalAccountsVerified->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMarkEmailExternalAccountsVerified), u"Accounts_Verify_Email_For_External_Accounts"_s);

    auto mManualApproveNewUser = w.findChild<QCheckBox *>(u"mManualApproveNewUser"_s);
    QVERIFY(mManualApproveNewUser);
    QVERIFY(!mManualApproveNewUser->isChecked());
    QVERIFY(!mManualApproveNewUser->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mManualApproveNewUser), u"Accounts_ManuallyApproveNewUsers"_s);

    auto mDefaultUsernamePrefixSuggestion = w.findChild<QLineEdit *>(u"mDefaultUsernamePrefixSuggestion"_s);
    QVERIFY(mDefaultUsernamePrefixSuggestion);
    QVERIFY(mDefaultUsernamePrefixSuggestion->text().isEmpty());
    QVERIFY(mDefaultUsernamePrefixSuggestion->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDefaultUsernamePrefixSuggestion), u"Accounts_DefaultUsernamePrefixSuggestion"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_DefaultUsernamePrefixSuggestion"_s);

    auto mAllowedDomainsList = w.findChild<QLineEdit *>(u"mAllowedDomainsList"_s);
    QVERIFY(mAllowedDomainsList);
    QVERIFY(mAllowedDomainsList->text().isEmpty());
    QVERIFY(!mAllowedDomainsList->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowedDomainsList), u"Accounts_AllowedDomainsList"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_AllowedDomainsList"_s);

    auto mBlockedDomainsList = w.findChild<QLineEdit *>(u"mBlockedDomainsList"_s);
    QVERIFY(mBlockedDomainsList);
    QVERIFY(mBlockedDomainsList->text().isEmpty());
    QVERIFY(!mBlockedDomainsList->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mBlockedDomainsList), u"Accounts_BlockedDomainsList"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_BlockedDomainsList"_s);

    auto mBlockedUsernameList = w.findChild<QLineEdit *>(u"mBlockedUsernameList"_s);
    QVERIFY(mBlockedUsernameList);
    QVERIFY(mBlockedUsernameList->text().isEmpty());
    QVERIFY(!mBlockedUsernameList->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mBlockedUsernameList), u"Accounts_BlockedUsernameList"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_BlockedUsernameList"_s);

    auto mUseDefaultBlockedDomainsList = w.findChild<QCheckBox *>(u"mUseDefaultBlockedDomainsList"_s);
    QVERIFY(mUseDefaultBlockedDomainsList);
    QVERIFY(!mUseDefaultBlockedDomainsList->isChecked());
    QVERIFY(!mUseDefaultBlockedDomainsList->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUseDefaultBlockedDomainsList), u"Accounts_UseDefaultBlockedDomainsList"_s);

    auto mUseDNSDomainCheck = w.findChild<QCheckBox *>(u"mUseDNSDomainCheck"_s);
    QVERIFY(mUseDNSDomainCheck);
    QVERIFY(!mUseDNSDomainCheck->isChecked());
    QVERIFY(!mUseDNSDomainCheck->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUseDNSDomainCheck), u"Accounts_UseDNSDomainCheck"_s);

    auto mAllowFeaturePreview = w.findChild<QCheckBox *>(u"mAllowFeaturePreview"_s);
    QVERIFY(mAllowFeaturePreview);
    QVERIFY(!mAllowFeaturePreview->isChecked());
    QVERIFY(!mAllowFeaturePreview->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowFeaturePreview), u"Accounts_AllowFeaturePreview"_s);

    auto mRegistrationAuthenticationServices = w.findChild<QCheckBox *>(u"mRegistrationAuthenticationServices"_s);
    QVERIFY(mRegistrationAuthenticationServices);
    QVERIFY(!mRegistrationAuthenticationServices->isChecked());
    QVERIFY(!mRegistrationAuthenticationServices->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mRegistrationAuthenticationServices), u"Accounts_Registration_AuthenticationServices_Enabled"_s);

    auto mRegistrationAuthenticationServicesDefaultRoles = w.findChild<QLineEdit *>(u"mRegistrationAuthenticationServicesDefaultRoles"_s);
    QVERIFY(mRegistrationAuthenticationServicesDefaultRoles);
    QVERIFY(mRegistrationAuthenticationServicesDefaultRoles->text().isEmpty());
    QVERIFY(!mRegistrationAuthenticationServicesDefaultRoles->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mRegistrationAuthenticationServicesDefaultRoles),
             u"Accounts_Registration_AuthenticationServices_Default_Roles"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_Registration_AuthenticationServices_Default_Roles"_s);

    auto mRegistrationUsersDefaultRoles = w.findChild<QLineEdit *>(u"mRegistrationUsersDefaultRoles"_s);
    QVERIFY(mRegistrationUsersDefaultRoles);
    QVERIFY(mRegistrationUsersDefaultRoles->text().isEmpty());
    QVERIFY(!mRegistrationUsersDefaultRoles->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mRegistrationUsersDefaultRoles), u"Accounts_Registration_Users_Default_Roles"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_Registration_Users_Default_Roles"_s);

    auto mPasswordReset = w.findChild<QCheckBox *>(u"mPasswordReset"_s);
    QVERIFY(mPasswordReset);
    QVERIFY(!mPasswordReset->isChecked());
    QVERIFY(!mPasswordReset->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPasswordReset), u"Accounts_PasswordReset"_s);

    auto loginLogsLabel = w.findChild<QLabel *>(u"loginLogsLabel"_s);
    QVERIFY(loginLogsLabel);
    QVERIFY(!loginLogsLabel->text().isEmpty());

    auto mLoginLogs = w.findChild<QCheckBox *>(u"mLoginLogs"_s);
    QVERIFY(mLoginLogs);
    QVERIFY(!mLoginLogs->isChecked());
    QVERIFY(!mLoginLogs->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLoginLogs), u"Login_Logs_Enabled"_s);

    auto mLoginLogsUsername = w.findChild<QCheckBox *>(u"mLoginLogsUsername"_s);
    QVERIFY(mLoginLogsUsername);
    QVERIFY(!mLoginLogsUsername->isChecked());
    QVERIFY(!mLoginLogsUsername->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLoginLogsUsername), u"Login_Logs_Username"_s);

    auto mLoginLogsUseragent = w.findChild<QCheckBox *>(u"mLoginLogsUseragent"_s);
    QVERIFY(mLoginLogsUseragent);
    QVERIFY(!mLoginLogsUseragent->isChecked());
    QVERIFY(!mLoginLogsUseragent->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLoginLogsUseragent), u"Login_Logs_UserAgent"_s);

    auto mLoginLogsClientIp = w.findChild<QCheckBox *>(u"mLoginLogsClientIp"_s);
    QVERIFY(mLoginLogsClientIp);
    QVERIFY(!mLoginLogsClientIp->isChecked());
    QVERIFY(!mLoginLogsClientIp->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLoginLogsClientIp), u"Login_Logs_ClientIp"_s);

    auto mLoginLogsForwardedForIp = w.findChild<QCheckBox *>(u"mLoginLogsForwardedForIp"_s);
    QVERIFY(mLoginLogsForwardedForIp);
    QVERIFY(!mLoginLogsForwardedForIp->isChecked());
    QVERIFY(!mLoginLogsForwardedForIp->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLoginLogsForwardedForIp), u"mLoginLogsForwardedForIp"_s);
}

#include "moc_accountsettingswidgettest.cpp"
