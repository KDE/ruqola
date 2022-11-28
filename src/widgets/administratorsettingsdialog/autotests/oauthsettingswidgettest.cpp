/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthsettingswidgettest.h"
#include "administratorsettingsdialog/oauth/oauthsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(OauthSettingsWidgetTest)
OauthSettingsWidgetTest::OauthSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void OauthSettingsWidgetTest::shouldHaveDefaultValues()
{
    OauthSettingsWidget w(nullptr);
    auto mSignWithApple = w.findChild<QCheckBox *>(QStringLiteral("mSignWithApple"));
    QVERIFY(mSignWithApple);
    QVERIFY(!mSignWithApple->isChecked());
    QVERIFY(!mSignWithApple->text().isEmpty());
    QVERIFY(mSignWithApple->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSignWithApple), QStringLiteral("Accounts_OAuth_Apple"));

    auto mAccountsOAuthAppleId = w.findChild<QLineEdit *>(QStringLiteral("mAccountsOAuthAppleId"));
    QVERIFY(mAccountsOAuthAppleId);
    QVERIFY(mAccountsOAuthAppleId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAccountsOAuthAppleId), QStringLiteral("Accounts_OAuth_Apple_id"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Apple_id"));

    auto mAccountsOAuthAppleIss = w.findChild<QLineEdit *>(QStringLiteral("mAccountsOAuthAppleIss"));
    QVERIFY(mAccountsOAuthAppleIss);
    QVERIFY(mAccountsOAuthAppleIss->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAccountsOAuthAppleIss), QStringLiteral("Accounts_OAuth_Apple_iss"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Apple_iss"));

    auto AccountsOAuthAppleKid = w.findChild<QLineEdit *>(QStringLiteral("AccountsOAuthAppleKid"));
    QVERIFY(AccountsOAuthAppleKid);
    QVERIFY(AccountsOAuthAppleKid->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(AccountsOAuthAppleKid), QStringLiteral("Accounts_OAuth_Apple_kid"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Apple_kid"));

    auto mTwitterLogin = w.findChild<QCheckBox *>(QStringLiteral("mSignWithApple"));
    QVERIFY(mTwitterLogin);
    QVERIFY(!mTwitterLogin->isChecked());
    QVERIFY(!mTwitterLogin->text().isEmpty());
    QVERIFY(mTwitterLogin->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTwitterLogin), QStringLiteral("Accounts_OAuth_Twitter"));

    auto mTwitterId = w.findChild<QLineEdit *>(QStringLiteral("mTwitterId"));
    QVERIFY(mTwitterId);
    QVERIFY(mTwitterId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTwitterId), QStringLiteral("Accounts_OAuth_Twitter_id"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Twitter_id"));

    auto mTwitterSecret = w.findChild<QLineEdit *>(QStringLiteral("mTwitterSecret"));
    QVERIFY(mTwitterSecret);
    QVERIFY(mTwitterSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTwitterSecret), QStringLiteral("Accounts_OAuth_Twitter_secret"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Twitter_secret"));
}
