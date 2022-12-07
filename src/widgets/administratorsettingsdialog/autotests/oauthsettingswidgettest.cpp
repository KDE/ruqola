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

    auto mFacebookLogin = w.findChild<QCheckBox *>(QStringLiteral("mFacebookLogin"));
    QVERIFY(mFacebookLogin);
    QVERIFY(!mFacebookLogin->isChecked());
    QVERIFY(!mFacebookLogin->text().isEmpty());
    QVERIFY(mFacebookLogin->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFacebookLogin), QStringLiteral("Accounts_OAuth_Facebook"));

    auto mFacebookId = w.findChild<QLineEdit *>(QStringLiteral("mFacebookId"));
    QVERIFY(mFacebookId);
    QVERIFY(mFacebookId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFacebookId), QStringLiteral("Accounts_OAuth_Facebook_id"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Facebook_id"));

    auto mFacebookSecret = w.findChild<QLineEdit *>(QStringLiteral("mFacebookSecret"));
    QVERIFY(mFacebookSecret);
    QVERIFY(mFacebookSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFacebookSecret), QStringLiteral("Accounts_OAuth_Facebook_secret"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Facebook_secret"));

    auto mFacebookCallbackURL = w.findChild<QLineEdit *>(QStringLiteral("mFacebookCallbackURL"));
    QVERIFY(mFacebookCallbackURL);
    QVERIFY(mFacebookCallbackURL->text().isEmpty());
    QVERIFY(mFacebookCallbackURL->isReadOnly());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFacebookCallbackURL), QStringLiteral("Accounts_OAuth_Facebook_callback_url"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Facebook_callback_url"));

    auto mGithubLogin = w.findChild<QCheckBox *>(QStringLiteral("mGithubLogin"));
    QVERIFY(mGithubLogin);
    QVERIFY(!mGithubLogin->isChecked());
    QVERIFY(!mGithubLogin->text().isEmpty());
    QVERIFY(mGithubLogin->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGithubLogin), QStringLiteral("Accounts_OAuth_Github"));

    auto mGithubId = w.findChild<QLineEdit *>(QStringLiteral("mGithubId"));
    QVERIFY(mGithubId);
    QVERIFY(mGithubId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGithubId), QStringLiteral("Accounts_OAuth_Github_id"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Github_id"));

    auto mGithubSecret = w.findChild<QLineEdit *>(QStringLiteral("mGithubSecret"));
    QVERIFY(mGithubSecret);
    QVERIFY(mGithubSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGithubSecret), QStringLiteral("Accounts_OAuth_Github_secret"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Github_secret"));

    auto mGithubCallbackURL = w.findChild<QLineEdit *>(QStringLiteral("mGithubCallbackURL"));
    QVERIFY(mGithubCallbackURL);
    QVERIFY(mGithubCallbackURL->text().isEmpty());
    QVERIFY(mGithubCallbackURL->isReadOnly());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGithubCallbackURL), QStringLiteral("Accounts_OAuth_Github_callback_url"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Github_callback_url"));

    auto mTokenpassLogin = w.findChild<QCheckBox *>(QStringLiteral("mTokenpassLogin"));
    QVERIFY(mTokenpassLogin);
    QVERIFY(!mTokenpassLogin->isChecked());
    QVERIFY(!mTokenpassLogin->text().isEmpty());
    QVERIFY(mTokenpassLogin->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTokenpassLogin), QStringLiteral("Accounts_OAuth_Tokenpass"));

    auto mTokenpassId = w.findChild<QLineEdit *>(QStringLiteral("mTokenpassId"));
    QVERIFY(mTokenpassId);
    QVERIFY(mTokenpassId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTokenpassId), QStringLiteral("Accounts_OAuth_Tokenpass_id"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Tokenpass_id"));

    auto mTokenpassSecret = w.findChild<QLineEdit *>(QStringLiteral("mTokenpassSecret"));
    QVERIFY(mTokenpassSecret);
    QVERIFY(mTokenpassSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTokenpassSecret), QStringLiteral("Accounts_OAuth_Tokenpass_secret"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Tokenpass_secret"));

    auto mTokenpassCallbackURL = w.findChild<QLineEdit *>(QStringLiteral("mTokenpassCallbackURL"));
    QVERIFY(mTokenpassCallbackURL);
    QVERIFY(mTokenpassCallbackURL->text().isEmpty());
    QVERIFY(mTokenpassCallbackURL->isReadOnly());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTokenpassCallbackURL), QStringLiteral("Accounts_OAuth_Tokenpass_callback_url"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Tokenpass_callback_url"));

    auto mTokenpassServerURL = w.findChild<QLineEdit *>(QStringLiteral("mTokenpassServerURL"));
    QVERIFY(mTokenpassServerURL);
    QVERIFY(mTokenpassServerURL->text().isEmpty());
    QVERIFY(!mTokenpassServerURL->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTokenpassServerURL), QStringLiteral("API_Tokenpass_URL"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("API_Tokenpass_URL"));

    auto mGoogleLogin = w.findChild<QCheckBox *>(QStringLiteral("mGoogleLogin"));
    QVERIFY(mGoogleLogin);
    QVERIFY(!mGoogleLogin->isChecked());
    QVERIFY(!mGoogleLogin->text().isEmpty());
    QVERIFY(mGoogleLogin->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGoogleLogin), QStringLiteral("Accounts_OAuth_Google"));

    auto mGoogleId = w.findChild<QLineEdit *>(QStringLiteral("mGoogleId"));
    QVERIFY(mGoogleId);
    QVERIFY(mGoogleId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGoogleId), QStringLiteral("Accounts_OAuth_Google_id"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Google_id"));

    auto mGoogleSecret = w.findChild<QLineEdit *>(QStringLiteral("mGoogleSecret"));
    QVERIFY(mGoogleSecret);
    QVERIFY(mGoogleSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGoogleSecret), QStringLiteral("Accounts_OAuth_Google_secret"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Google_secret"));

    auto mGoogleCallbackURL = w.findChild<QLineEdit *>(QStringLiteral("mGoogleCallbackURL"));
    QVERIFY(mGoogleCallbackURL);
    QVERIFY(mGoogleCallbackURL->text().isEmpty());
    QVERIFY(mGoogleCallbackURL->isReadOnly());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGoogleCallbackURL), QStringLiteral("Accounts_OAuth_Google_callback_url"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Google_callback_url"));

    auto mDolphinUrl = w.findChild<QLineEdit *>(QStringLiteral("mDolphinUrl"));
    QVERIFY(mDolphinUrl);
    QVERIFY(mDolphinUrl->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDolphinUrl), QStringLiteral("Accounts_OAuth_Dolphin_URL"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Dolphin_URL"));

    auto mDolphinEnable = w.findChild<QCheckBox *>(QStringLiteral("mDolphinEnable"));
    QVERIFY(mDolphinEnable);
    QVERIFY(!mDolphinEnable->isChecked());
    QVERIFY(!mDolphinEnable->text().isEmpty());
    QVERIFY(mDolphinEnable->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDolphinEnable), QStringLiteral("Accounts_OAuth_Dolphin"));

    auto mDolphinId = w.findChild<QLineEdit *>(QStringLiteral("mDolphinId"));
    QVERIFY(mDolphinId);
    QVERIFY(mDolphinId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDolphinId), QStringLiteral("Accounts_OAuth_Dolphin_id"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Dolphin_id"));

    auto mDolphinSecret = w.findChild<QLineEdit *>(QStringLiteral("mDolphinSecret"));
    QVERIFY(mDolphinSecret);
    QVERIFY(mDolphinSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDolphinSecret), QStringLiteral("Accounts_OAuth_Dolphin_secret"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Dolphin_secret"));
}
