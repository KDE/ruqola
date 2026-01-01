/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthsettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/oauth/oauthsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QLineEdit>
QTEST_MAIN(OauthSettingsWidgetTest)
OauthSettingsWidgetTest::OauthSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void OauthSettingsWidgetTest::shouldHaveDefaultValues()
{
    OauthSettingsWidget w(nullptr);
    auto mSignWithApple = w.findChild<QCheckBox *>(u"mSignWithApple"_s);
    QVERIFY(mSignWithApple);
    QVERIFY(!mSignWithApple->isChecked());
    QVERIFY(!mSignWithApple->text().isEmpty());
    QVERIFY(!mSignWithApple->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSignWithApple), u"Accounts_OAuth_Apple"_s);

    auto mAccountsOAuthAppleId = w.findChild<QLineEdit *>(u"mAccountsOAuthAppleId"_s);
    QVERIFY(mAccountsOAuthAppleId);
    QVERIFY(mAccountsOAuthAppleId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAccountsOAuthAppleId), u"Accounts_OAuth_Apple_id"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Apple_id"_s);

    auto mAccountsOAuthAppleIss = w.findChild<QLineEdit *>(u"mAccountsOAuthAppleIss"_s);
    QVERIFY(mAccountsOAuthAppleIss);
    QVERIFY(mAccountsOAuthAppleIss->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAccountsOAuthAppleIss), u"Accounts_OAuth_Apple_iss"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Apple_iss"_s);

    auto AccountsOAuthAppleKid = w.findChild<QLineEdit *>(u"AccountsOAuthAppleKid"_s);
    QVERIFY(AccountsOAuthAppleKid);
    QVERIFY(AccountsOAuthAppleKid->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(AccountsOAuthAppleKid), u"Accounts_OAuth_Apple_kid"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Apple_kid"_s);

    auto mTwitterLogin = w.findChild<QCheckBox *>(u"mTwitterLogin"_s);
    QVERIFY(mTwitterLogin);
    QVERIFY(!mTwitterLogin->isChecked());
    QVERIFY(!mTwitterLogin->text().isEmpty());
    QVERIFY(mTwitterLogin->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTwitterLogin), u"Accounts_OAuth_Twitter"_s);

    auto mTwitterId = w.findChild<QLineEdit *>(u"mTwitterId"_s);
    QVERIFY(mTwitterId);
    QVERIFY(mTwitterId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTwitterId), u"Accounts_OAuth_Twitter_id"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Twitter_id"_s);

    auto mTwitterSecret = w.findChild<QLineEdit *>(u"mTwitterSecret"_s);
    QVERIFY(mTwitterSecret);
    QVERIFY(mTwitterSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTwitterSecret), u"Accounts_OAuth_Twitter_secret"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Twitter_secret"_s);

    auto mTwitterCallbackURL = w.findChild<QLineEdit *>(u"mTwitterCallbackURL"_s);
    QVERIFY(mTwitterCallbackURL);
    QVERIFY(mTwitterCallbackURL->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTwitterCallbackURL), u"Accounts_OAuth_Twitter_callback_url"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Twitter_callback_url"_s);

    auto mLinkedInLogin = w.findChild<QCheckBox *>(u"mLinkedInLogin"_s);
    QVERIFY(mLinkedInLogin);
    QVERIFY(!mLinkedInLogin->isChecked());
    QVERIFY(!mLinkedInLogin->text().isEmpty());
    QVERIFY(mLinkedInLogin->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLinkedInLogin), u"Accounts_OAuth_Linkedin"_s);

    auto mLinkedInId = w.findChild<QLineEdit *>(u"mLinkedInId"_s);
    QVERIFY(mLinkedInId);
    QVERIFY(mLinkedInId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLinkedInId), u"Accounts_OAuth_Linkedin_id"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Linkedin_id"_s);

    auto mLinkedInSecret = w.findChild<QLineEdit *>(u"mLinkedInSecret"_s);
    QVERIFY(mLinkedInSecret);
    QVERIFY(mLinkedInSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLinkedInSecret), u"Accounts_OAuth_Linkedin_secret"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Linkedin_secret"_s);

    auto mLinkedInCallbackURL = w.findChild<QLineEdit *>(u"mLinkedInCallbackURL"_s);
    QVERIFY(mLinkedInCallbackURL);
    QVERIFY(mLinkedInCallbackURL->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mLinkedInCallbackURL), u"Accounts_OAuth_Linkedin_callback_url"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Linkedin_callback_url"_s);

    auto mProxyHost = w.findChild<QLineEdit *>(u"mProxyHost"_s);
    QVERIFY(mProxyHost);
    QVERIFY(mProxyHost->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mProxyHost), u"Accounts_OAuth_Proxy_host"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Proxy_host"_s);

    auto mProxyServices = w.findChild<QLineEdit *>(u"mProxyServices"_s);
    QVERIFY(mProxyServices);
    QVERIFY(mProxyServices->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mProxyServices), u"Accounts_OAuth_Proxy_services"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Proxy_services"_s);

    auto mMeteorLogin = w.findChild<QCheckBox *>(u"mMeteorLogin"_s);
    QVERIFY(mMeteorLogin);
    QVERIFY(!mMeteorLogin->isChecked());
    QVERIFY(!mMeteorLogin->text().isEmpty());
    QVERIFY(mMeteorLogin->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMeteorLogin), u"Accounts_OAuth_Meteor"_s);

    auto mMeteorId = w.findChild<QLineEdit *>(u"mMeteorId"_s);
    QVERIFY(mMeteorId);
    QVERIFY(mMeteorId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMeteorId), u"Accounts_OAuth_Meteor_id"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Meteor_id"_s);

    auto mMeteorSecret = w.findChild<QLineEdit *>(u"mMeteorSecret"_s);
    QVERIFY(mMeteorSecret);
    QVERIFY(mMeteorSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMeteorSecret), u"Accounts_OAuth_Meteor_secret"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Meteor_secret"_s);

    auto mMeteorCallbackURL = w.findChild<QLineEdit *>(u"mMeteorCallbackURL"_s);
    QVERIFY(mMeteorCallbackURL);
    QVERIFY(mMeteorCallbackURL->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMeteorCallbackURL), u"Accounts_OAuth_Meteor_callback_url"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Meteor_callback_url"_s);

    auto mFacebookLogin = w.findChild<QCheckBox *>(u"mFacebookLogin"_s);
    QVERIFY(mFacebookLogin);
    QVERIFY(!mFacebookLogin->isChecked());
    QVERIFY(!mFacebookLogin->text().isEmpty());
    QVERIFY(mFacebookLogin->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFacebookLogin), u"Accounts_OAuth_Facebook"_s);

    auto mFacebookId = w.findChild<QLineEdit *>(u"mFacebookId"_s);
    QVERIFY(mFacebookId);
    QVERIFY(mFacebookId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFacebookId), u"Accounts_OAuth_Facebook_id"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Facebook_id"_s);

    auto mFacebookSecret = w.findChild<QLineEdit *>(u"mFacebookSecret"_s);
    QVERIFY(mFacebookSecret);
    QVERIFY(mFacebookSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFacebookSecret), u"Accounts_OAuth_Facebook_secret"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Facebook_secret"_s);

    auto mFacebookCallbackURL = w.findChild<QLineEdit *>(u"mFacebookCallbackURL"_s);
    QVERIFY(mFacebookCallbackURL);
    QVERIFY(mFacebookCallbackURL->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFacebookCallbackURL), u"Accounts_OAuth_Facebook_callback_url"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Facebook_callback_url"_s);

    auto mGithubLogin = w.findChild<QCheckBox *>(u"mGithubLogin"_s);
    QVERIFY(mGithubLogin);
    QVERIFY(!mGithubLogin->isChecked());
    QVERIFY(!mGithubLogin->text().isEmpty());
    QVERIFY(mGithubLogin->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGithubLogin), u"Accounts_OAuth_Github"_s);

    auto mGithubId = w.findChild<QLineEdit *>(u"mGithubId"_s);
    QVERIFY(mGithubId);
    QVERIFY(mGithubId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGithubId), u"Accounts_OAuth_Github_id"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Github_id"_s);

    auto mGithubSecret = w.findChild<QLineEdit *>(u"mGithubSecret"_s);
    QVERIFY(mGithubSecret);
    QVERIFY(mGithubSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGithubSecret), u"Accounts_OAuth_Github_secret"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Github_secret"_s);

    auto mGithubCallbackURL = w.findChild<QLineEdit *>(u"mGithubCallbackURL"_s);
    QVERIFY(mGithubCallbackURL);
    QVERIFY(mGithubCallbackURL->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGithubCallbackURL), u"Accounts_OAuth_Github_callback_url"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Github_callback_url"_s);

    auto mTokenpassLogin = w.findChild<QCheckBox *>(u"mTokenpassLogin"_s);
    QVERIFY(mTokenpassLogin);
    QVERIFY(!mTokenpassLogin->isChecked());
    QVERIFY(!mTokenpassLogin->text().isEmpty());
    QVERIFY(mTokenpassLogin->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTokenpassLogin), u"Accounts_OAuth_Tokenpass"_s);

    auto mTokenpassId = w.findChild<QLineEdit *>(u"mTokenpassId"_s);
    QVERIFY(mTokenpassId);
    QVERIFY(mTokenpassId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTokenpassId), u"Accounts_OAuth_Tokenpass_id"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Tokenpass_id"_s);

    auto mTokenpassSecret = w.findChild<QLineEdit *>(u"mTokenpassSecret"_s);
    QVERIFY(mTokenpassSecret);
    QVERIFY(mTokenpassSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTokenpassSecret), u"Accounts_OAuth_Tokenpass_secret"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Tokenpass_secret"_s);

    auto mTokenpassCallbackURL = w.findChild<QLineEdit *>(u"mTokenpassCallbackURL"_s);
    QVERIFY(mTokenpassCallbackURL);
    QVERIFY(mTokenpassCallbackURL->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTokenpassCallbackURL), u"Accounts_OAuth_Tokenpass_callback_url"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Tokenpass_callback_url"_s);

    auto mTokenpassServerURL = w.findChild<QLineEdit *>(u"mTokenpassServerURL"_s);
    QVERIFY(mTokenpassServerURL);
    QVERIFY(mTokenpassServerURL->text().isEmpty());
    QVERIFY(!mTokenpassServerURL->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTokenpassServerURL), u"API_Tokenpass_URL"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"API_Tokenpass_URL"_s);

    auto mGoogleLogin = w.findChild<QCheckBox *>(u"mGoogleLogin"_s);
    QVERIFY(mGoogleLogin);
    QVERIFY(!mGoogleLogin->isChecked());
    QVERIFY(!mGoogleLogin->text().isEmpty());
    QVERIFY(mGoogleLogin->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGoogleLogin), u"Accounts_OAuth_Google"_s);

    auto mGoogleId = w.findChild<QLineEdit *>(u"mGoogleId"_s);
    QVERIFY(mGoogleId);
    QVERIFY(mGoogleId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGoogleId), u"Accounts_OAuth_Google_id"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Google_id"_s);

    auto mGoogleSecret = w.findChild<QLineEdit *>(u"mGoogleSecret"_s);
    QVERIFY(mGoogleSecret);
    QVERIFY(mGoogleSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGoogleSecret), u"Accounts_OAuth_Google_secret"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Google_secret"_s);

    auto mGoogleCallbackURL = w.findChild<QLineEdit *>(u"mGoogleCallbackURL"_s);
    QVERIFY(mGoogleCallbackURL);
    QVERIFY(mGoogleCallbackURL->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGoogleCallbackURL), u"Accounts_OAuth_Google_callback_url"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Google_callback_url"_s);

    auto mDolphinUrl = w.findChild<QLineEdit *>(u"mDolphinUrl"_s);
    QVERIFY(mDolphinUrl);
    QVERIFY(mDolphinUrl->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDolphinUrl), u"Accounts_OAuth_Dolphin_URL"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Dolphin_URL"_s);

    auto mDolphinEnable = w.findChild<QCheckBox *>(u"mDolphinEnable"_s);
    QVERIFY(mDolphinEnable);
    QVERIFY(!mDolphinEnable->isChecked());
    QVERIFY(!mDolphinEnable->text().isEmpty());
    QVERIFY(mDolphinEnable->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDolphinEnable), u"Accounts_OAuth_Dolphin"_s);

    auto mDolphinId = w.findChild<QLineEdit *>(u"mDolphinId"_s);
    QVERIFY(mDolphinId);
    QVERIFY(mDolphinId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDolphinId), u"Accounts_OAuth_Dolphin_id"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Dolphin_id"_s);

    auto mDolphinSecret = w.findChild<QLineEdit *>(u"mDolphinSecret"_s);
    QVERIFY(mDolphinSecret);
    QVERIFY(mDolphinSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDolphinSecret), u"Accounts_OAuth_Dolphin_secret"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_OAuth_Dolphin_secret"_s);
}

#include "moc_oauthsettingswidgettest.cpp"
