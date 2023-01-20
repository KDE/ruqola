/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountsettingswidgettest.h"
#include "administratorsettingsdialog/accounts/accountsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QSpinBox>
#include <QTest>

QTEST_MAIN(AccountSettingsWidgetTest)

AccountSettingsWidgetTest::AccountSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AccountSettingsWidgetTest::shouldHaveDefaultValues()
{
    AccountSettingsWidget w(nullptr);
    QVERIFY(w.widgetResizable());

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mAllowChangeName = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangeName"));
    QVERIFY(mAllowChangeName);
    QVERIFY(!mAllowChangeName->isChecked());
    QVERIFY(!mAllowChangeName->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowChangeName), QStringLiteral("Accounts_AllowRealNameChange"));

    auto mAllowChangeUserProfile = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangeUserProfile"));
    QVERIFY(mAllowChangeUserProfile);
    QVERIFY(!mAllowChangeUserProfile->isChecked());
    QVERIFY(!mAllowChangeUserProfile->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowChangeUserProfile), QStringLiteral("Accounts_AllowUserProfileChange"));

    auto mAllowChangeUserAvatar = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangeUserAvatar"));
    QVERIFY(mAllowChangeUserAvatar);
    QVERIFY(!mAllowChangeUserAvatar->isChecked());
    QVERIFY(!mAllowChangeUserAvatar->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowChangeUserAvatar), QStringLiteral("Accounts_AllowUserAvatarChange"));

    auto mAllowChangeEmail = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangeEmail"));
    QVERIFY(mAllowChangeEmail);
    QVERIFY(!mAllowChangeEmail->isChecked());
    QVERIFY(!mAllowChangeEmail->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowChangeEmail), QStringLiteral("Accounts_AllowEmailChange"));

    auto mAllowChangePassword = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangePassword"));
    QVERIFY(mAllowChangePassword);
    QVERIFY(!mAllowChangePassword->isChecked());
    QVERIFY(!mAllowChangePassword->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowChangePassword), QStringLiteral("Accounts_AllowPasswordChange"));

    auto mAllowChangeEmailNotifications = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangeEmailNotifications"));
    QVERIFY(mAllowChangeEmailNotifications);
    QVERIFY(!mAllowChangeEmailNotifications->isChecked());
    QVERIFY(!mAllowChangeEmailNotifications->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowChangeEmailNotifications), QStringLiteral("Accounts_AllowEmailNotifications"));

    auto mAllowUsersToDeleteOwnAccount = w.findChild<QCheckBox *>(QStringLiteral("mAllowUsersToDeleteOwnAccount"));
    QVERIFY(mAllowUsersToDeleteOwnAccount);
    QVERIFY(!mAllowUsersToDeleteOwnAccount->isChecked());
    QVERIFY(!mAllowUsersToDeleteOwnAccount->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowUsersToDeleteOwnAccount), QStringLiteral("Accounts_AllowDeleteOwnAccount"));

    auto mAllowAnonymousWrite = w.findChild<QCheckBox *>(QStringLiteral("mAllowAnonymousWrite"));
    QVERIFY(mAllowAnonymousWrite);
    QVERIFY(!mAllowAnonymousWrite->isChecked());
    QVERIFY(!mAllowAnonymousWrite->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowAnonymousWrite), QStringLiteral("Accounts_AllowAnonymousWrite"));

    auto mAllowAnonymousRead = w.findChild<QCheckBox *>(QStringLiteral("mAllowAnonymousRead"));
    QVERIFY(mAllowAnonymousRead);
    QVERIFY(!mAllowAnonymousRead->isChecked());
    QVERIFY(!mAllowAnonymousRead->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowAnonymousRead), QStringLiteral("Accounts_AllowAnonymousRead"));

    auto mAllowUsersDeleteOwnAccount = w.findChild<QCheckBox *>(QStringLiteral("mAllowUsersDeleteOwnAccount"));
    QVERIFY(mAllowUsersDeleteOwnAccount);
    QVERIFY(!mAllowUsersDeleteOwnAccount->isChecked());
    QVERIFY(!mAllowUsersDeleteOwnAccount->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowUsersDeleteOwnAccount), QStringLiteral("Accounts_AllowDeleteOwnAccount"));

    auto mAllowPasswordChangeOauthUsers = w.findChild<QCheckBox *>(QStringLiteral("mAllowPasswordChangeOauthUsers"));
    QVERIFY(mAllowPasswordChangeOauthUsers);
    QVERIFY(!mAllowPasswordChangeOauthUsers->isChecked());
    QVERIFY(!mAllowPasswordChangeOauthUsers->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowPasswordChangeOauthUsers), QStringLiteral("Accounts_AllowPasswordChangeForOAuthUsers"));

    auto mLoginExpirationInDays = w.findChild<QSpinBox *>(QStringLiteral("mLoginExpirationInDays"));
    QVERIFY(mLoginExpirationInDays);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLoginExpirationInDays), QStringLiteral("Accounts_LoginExpiration"));

    auto mAllowInvisibleStatusOption = w.findChild<QCheckBox *>(QStringLiteral("mAllowInvisibleStatusOption"));
    QVERIFY(mAllowInvisibleStatusOption);
    QVERIFY(!mAllowInvisibleStatusOption->isChecked());
    QVERIFY(!mAllowInvisibleStatusOption->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowInvisibleStatusOption), QStringLiteral("Accounts_AllowInvisibleStatusOption"));

    auto mForgetUserSessionWindowClose = w.findChild<QCheckBox *>(QStringLiteral("mForgetUserSessionWindowClose"));
    QVERIFY(mForgetUserSessionWindowClose);
    QVERIFY(!mForgetUserSessionWindowClose->isChecked());
    QVERIFY(!mForgetUserSessionWindowClose->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mForgetUserSessionWindowClose), QStringLiteral("Accounts_ForgetUserSessionOnWindowClose"));

    auto failedLoginAttemptsLabel = w.findChild<QLabel *>(QStringLiteral("failedLoginAttemptsLabel"));
    QVERIFY(failedLoginAttemptsLabel);
    QVERIFY(!failedLoginAttemptsLabel->text().isEmpty());

    auto mEnableCollectLog = w.findChild<QCheckBox *>(QStringLiteral("mEnableCollectLog"));
    QVERIFY(mEnableCollectLog);
    QVERIFY(!mEnableCollectLog->isChecked());
    QVERIFY(!mEnableCollectLog->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableCollectLog), QStringLiteral("Block_Multiple_Failed_Logins_Enabled"));

    auto mAllowCustomStatusMessage = w.findChild<QCheckBox *>(QStringLiteral("mAllowCustomStatusMessage"));
    QVERIFY(mAllowCustomStatusMessage);
    QVERIFY(!mAllowCustomStatusMessage->isChecked());
    QVERIFY(!mAllowCustomStatusMessage->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowCustomStatusMessage), QStringLiteral("Accounts_AllowUserStatusMessageChange"));

    auto mBlockFailedLoginAttemptsUsername = w.findChild<QCheckBox *>(QStringLiteral("mBlockFailedLoginAttemptsUsername"));
    QVERIFY(mBlockFailedLoginAttemptsUsername);
    QVERIFY(!mBlockFailedLoginAttemptsUsername->isChecked());
    QVERIFY(!mBlockFailedLoginAttemptsUsername->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mBlockFailedLoginAttemptsUsername), QStringLiteral("Block_Multiple_Failed_Logins_By_User"));

    auto mHowManyFailedAttemptsUntilBlockUser = w.findChild<QSpinBox *>(QStringLiteral("mHowManyFailedAttemptsUntilBlockUser"));
    QVERIFY(mHowManyFailedAttemptsUntilBlockUser);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mHowManyFailedAttemptsUntilBlockUser),
             QStringLiteral("Block_Multiple_Failed_Logins_Attempts_Until_Block_by_User"));

    auto mTimeUnblockUser = w.findChild<QSpinBox *>(QStringLiteral("mTimeUnblockUser"));
    QVERIFY(mTimeUnblockUser);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTimeUnblockUser), QStringLiteral("Block_Multiple_Failed_Logins_Time_To_Unblock_By_User_In_Minutes"));

    auto mSendEmailUserWhenUserActivated = w.findChild<QCheckBox *>(QStringLiteral("mSendEmailUserWhenUserActivated"));
    QVERIFY(mSendEmailUserWhenUserActivated);
    QVERIFY(!mSendEmailUserWhenUserActivated->isChecked());
    QVERIFY(!mSendEmailUserWhenUserActivated->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSendEmailUserWhenUserActivated), QStringLiteral("Accounts_Send_Email_When_Activating"));

    auto mSendEmailUserWhenUserDeactivated = w.findChild<QCheckBox *>(QStringLiteral("mSendEmailUserWhenUserDeactivated"));
    QVERIFY(mSendEmailUserWhenUserDeactivated);
    QVERIFY(!mSendEmailUserWhenUserDeactivated->isChecked());
    QVERIFY(!mSendEmailUserWhenUserDeactivated->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSendEmailUserWhenUserDeactivated), QStringLiteral("Accounts_Send_Email_When_Deactivating"));

    auto mRequireNameSignup = w.findChild<QCheckBox *>(QStringLiteral("mRequireNameSignup"));
    QVERIFY(mRequireNameSignup);
    QVERIFY(!mRequireNameSignup->isChecked());
    QVERIFY(!mRequireNameSignup->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mRequireNameSignup), QStringLiteral("Accounts_RequireNameForSignUp"));

    auto mRequirePasswordConfirmation = w.findChild<QCheckBox *>(QStringLiteral("mRequirePasswordConfirmation"));
    QVERIFY(mRequirePasswordConfirmation);
    QVERIFY(!mRequirePasswordConfirmation->isChecked());
    QVERIFY(!mRequirePasswordConfirmation->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mRequirePasswordConfirmation), QStringLiteral("Accounts_RequirePasswordConfirmation"));
}
