/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountsettingswidgettest.h"
#include "administratorsettingsdialog/accounts/accountsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QFormLayout>
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
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAllowChangeName = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangeName"));
    QVERIFY(mAllowChangeName);
    QVERIFY(!mAllowChangeName->isChecked());
    QVERIFY(!mAllowChangeName->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mAllowChangeName), QStringLiteral("Accounts_AllowRealNameChange"));

    auto mAllowChangeUserProfile = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangeUserProfile"));
    QVERIFY(mAllowChangeUserProfile);
    QVERIFY(!mAllowChangeUserProfile->isChecked());
    QVERIFY(!mAllowChangeUserProfile->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mAllowChangeUserProfile), QStringLiteral("Accounts_AllowUserProfileChange"));

    auto mAllowChangeUserAvatar = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangeUserAvatar"));
    QVERIFY(mAllowChangeUserAvatar);
    QVERIFY(!mAllowChangeUserAvatar->isChecked());
    QVERIFY(!mAllowChangeUserAvatar->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mAllowChangeUserAvatar), QStringLiteral("Accounts_AllowUserAvatarChange"));

    auto mAllowChangeEmail = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangeEmail"));
    QVERIFY(mAllowChangeEmail);
    QVERIFY(!mAllowChangeEmail->isChecked());
    QVERIFY(!mAllowChangeEmail->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mAllowChangeEmail), QStringLiteral("Accounts_AllowEmailChange"));

    auto mAllowChangePassword = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangePassword"));
    QVERIFY(mAllowChangePassword);
    QVERIFY(!mAllowChangePassword->isChecked());
    QVERIFY(!mAllowChangePassword->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mAllowChangePassword), QStringLiteral("Accounts_AllowPasswordChange"));

    auto mAllowChangeEmailNotifications = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangeEmailNotifications"));
    QVERIFY(mAllowChangeEmailNotifications);
    QVERIFY(!mAllowChangeEmailNotifications->isChecked());
    QVERIFY(!mAllowChangeEmailNotifications->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mAllowChangeEmailNotifications), QStringLiteral("Accounts_AllowEmailNotifications"));

    auto mAllowUsersToDeleteOwnAccount = w.findChild<QCheckBox *>(QStringLiteral("mAllowUsersToDeleteOwnAccount"));
    QVERIFY(mAllowUsersToDeleteOwnAccount);
    QVERIFY(!mAllowUsersToDeleteOwnAccount->isChecked());
    QVERIFY(!mAllowUsersToDeleteOwnAccount->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mAllowUsersToDeleteOwnAccount), QStringLiteral("Accounts_AllowDeleteOwnAccount"));

    auto mAllowAnonymousWrite = w.findChild<QCheckBox *>(QStringLiteral("mAllowAnonymousWrite"));
    QVERIFY(mAllowAnonymousWrite);
    QVERIFY(!mAllowAnonymousWrite->isChecked());
    QVERIFY(!mAllowAnonymousWrite->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mAllowAnonymousWrite), QStringLiteral("Accounts_AllowAnonymousWrite"));

    auto mAllowUsersDeleteOwnAccount = w.findChild<QCheckBox *>(QStringLiteral("mAllowUsersDeleteOwnAccount"));
    QVERIFY(mAllowUsersDeleteOwnAccount);
    QVERIFY(!mAllowUsersDeleteOwnAccount->isChecked());
    QVERIFY(!mAllowUsersDeleteOwnAccount->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mAllowUsersDeleteOwnAccount), QStringLiteral("Accounts_AllowDeleteOwnAccount"));

    auto mAllowPasswordChangeOauthUsers = w.findChild<QCheckBox *>(QStringLiteral("mAllowPasswordChangeOauthUsers"));
    QVERIFY(mAllowPasswordChangeOauthUsers);
    QVERIFY(!mAllowPasswordChangeOauthUsers->isChecked());
    QVERIFY(!mAllowPasswordChangeOauthUsers->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mAllowPasswordChangeOauthUsers), QStringLiteral("Accounts_AllowPasswordChangeForOAuthUsers"));
}
