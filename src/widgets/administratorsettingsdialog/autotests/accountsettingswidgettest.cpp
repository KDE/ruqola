/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountsettingswidgettest.h"
#include "administratorsettingsdialog/accounts/accountsettingswidget.h"
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

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAllowChangeName = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangeName"));
    QVERIFY(mAllowChangeName);
    QVERIFY(!mAllowChangeName->isChecked());
    QVERIFY(!mAllowChangeName->text().isEmpty());

    auto mAllowChangeUserProfile = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangeUserProfile"));
    QVERIFY(mAllowChangeUserProfile);
    QVERIFY(!mAllowChangeUserProfile->isChecked());
    QVERIFY(!mAllowChangeUserProfile->text().isEmpty());

    auto mAllowChangeUserAvatar = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangeUserAvatar"));
    QVERIFY(mAllowChangeUserAvatar);
    QVERIFY(!mAllowChangeUserAvatar->isChecked());
    QVERIFY(!mAllowChangeUserAvatar->text().isEmpty());

    auto mAllowChangeEmail = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangeEmail"));
    QVERIFY(mAllowChangeEmail);
    QVERIFY(!mAllowChangeEmail->isChecked());
    QVERIFY(!mAllowChangeEmail->text().isEmpty());

    auto mAllowChangePassword = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangePassword"));
    QVERIFY(mAllowChangePassword);
    QVERIFY(!mAllowChangePassword->isChecked());
    QVERIFY(!mAllowChangePassword->text().isEmpty());

    auto mAllowChangeEmailNotifications = w.findChild<QCheckBox *>(QStringLiteral("mAllowChangeEmailNotifications"));
    QVERIFY(mAllowChangeEmailNotifications);
    QVERIFY(!mAllowChangeEmailNotifications->isChecked());
    QVERIFY(!mAllowChangeEmailNotifications->text().isEmpty());

    auto mAllowUsersToDeleteOwnAccount = w.findChild<QCheckBox *>(QStringLiteral("mAllowUsersToDeleteOwnAccount"));
    QVERIFY(mAllowUsersToDeleteOwnAccount);
    QVERIFY(!mAllowUsersToDeleteOwnAccount->isChecked());
    QVERIFY(!mAllowUsersToDeleteOwnAccount->text().isEmpty());

    auto mAllowAnonymousWrite = w.findChild<QCheckBox *>(QStringLiteral("mAllowAnonymousWrite"));
    QVERIFY(mAllowAnonymousWrite);
    QVERIFY(!mAllowAnonymousWrite->isChecked());
    QVERIFY(!mAllowAnonymousWrite->text().isEmpty());

    auto mAllowUsersDeleteOwnAccount = w.findChild<QCheckBox *>(QStringLiteral("mAllowUsersDeleteOwnAccount"));
    QVERIFY(mAllowUsersDeleteOwnAccount);
    QVERIFY(!mAllowUsersDeleteOwnAccount->isChecked());
    QVERIFY(!mAllowUsersDeleteOwnAccount->text().isEmpty());
}
