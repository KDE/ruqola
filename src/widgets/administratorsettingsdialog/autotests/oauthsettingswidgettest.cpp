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
    QVERIFY(!mSignWithApple->toolTip().isEmpty());
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

    auto mTwitterLogin = w.findChild<QCheckBox *>(QStringLiteral("mTwitterLogin"));
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

    auto mTwitterCallbackURL = w.findChild<QLineEdit *>(QStringLiteral("mTwitterCallbackURL"));
    QVERIFY(mTwitterCallbackURL);
    QVERIFY(mTwitterCallbackURL->text().isEmpty());
    QVERIFY(mTwitterCallbackURL->isReadOnly());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTwitterCallbackURL), QStringLiteral("Accounts_OAuth_Twitter_callback_url"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Twitter_callback_url"));

    auto mLinkedInLogin = w.findChild<QCheckBox *>(QStringLiteral("mLinkedInLogin"));
    QVERIFY(mLinkedInLogin);
    QVERIFY(!mLinkedInLogin->isChecked());
    QVERIFY(!mLinkedInLogin->text().isEmpty());
    QVERIFY(mLinkedInLogin->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLinkedInLogin), QStringLiteral("Accounts_OAuth_Linkedin"));

    auto mLinkedInId = w.findChild<QLineEdit *>(QStringLiteral("mLinkedInId"));
    QVERIFY(mLinkedInId);
    QVERIFY(mLinkedInId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLinkedInId), QStringLiteral("Accounts_OAuth_Linkedin_id"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Linkedin_id"));

    auto mLinkedInSecret = w.findChild<QLineEdit *>(QStringLiteral("mLinkedInSecret"));
    QVERIFY(mLinkedInSecret);
    QVERIFY(mLinkedInSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLinkedInSecret), QStringLiteral("Accounts_OAuth_Linkedin_secret"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Linkedin_secret"));

    auto mLinkedInCallbackURL = w.findChild<QLineEdit *>(QStringLiteral("mLinkedInCallbackURL"));
    QVERIFY(mLinkedInCallbackURL);
    QVERIFY(mLinkedInCallbackURL->text().isEmpty());
    QVERIFY(mLinkedInCallbackURL->isReadOnly());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLinkedInCallbackURL), QStringLiteral("Accounts_OAuth_Linkedin_callback_url"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Linkedin_callback_url"));

    auto mProxyHost = w.findChild<QLineEdit *>(QStringLiteral("mProxyHost"));
    QVERIFY(mProxyHost);
    QVERIFY(mProxyHost->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mProxyHost), QStringLiteral("Accounts_OAuth_Proxy_host"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Proxy_host"));

    auto mProxyServices = w.findChild<QLineEdit *>(QStringLiteral("mProxyServices"));
    QVERIFY(mProxyServices);
    QVERIFY(mProxyServices->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mProxyServices), QStringLiteral("Accounts_OAuth_Proxy_services"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Proxy_services"));

    auto mMeteorLogin = w.findChild<QCheckBox *>(QStringLiteral("mMeteorLogin"));
    QVERIFY(mMeteorLogin);
    QVERIFY(!mMeteorLogin->isChecked());
    QVERIFY(!mMeteorLogin->text().isEmpty());
    QVERIFY(mMeteorLogin->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMeteorLogin), QStringLiteral("Accounts_OAuth_Meteor"));

    auto mMeteorId = w.findChild<QLineEdit *>(QStringLiteral("mMeteorId"));
    QVERIFY(mMeteorId);
    QVERIFY(mMeteorId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMeteorId), QStringLiteral("Accounts_OAuth_Meteor_id"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Meteor_id"));

    auto mMeteorSecret = w.findChild<QLineEdit *>(QStringLiteral("mMeteorSecret"));
    QVERIFY(mMeteorSecret);
    QVERIFY(mMeteorSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMeteorSecret), QStringLiteral("Accounts_OAuth_Meteor_secret"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Meteor_secret"));

    auto mMeteorCallbackURL = w.findChild<QLineEdit *>(QStringLiteral("mMeteorCallbackURL"));
    QVERIFY(mMeteorCallbackURL);
    QVERIFY(mMeteorCallbackURL->text().isEmpty());
    QVERIFY(mMeteorCallbackURL->isReadOnly());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMeteorCallbackURL), QStringLiteral("Accounts_OAuth_Meteor_callback_url"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Meteor_callback_url"));
}
