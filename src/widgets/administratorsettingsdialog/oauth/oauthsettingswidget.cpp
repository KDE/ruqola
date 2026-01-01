/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthsettingswidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

OauthSettingsWidget::OauthSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mSignWithApple(new QCheckBox(i18nc("@option:check", "Sign in with Apple"), this))
    , mAccountsOAuthAppleId(new QLineEdit(this))
    , mAccountsOAuthAppleIss(new QLineEdit(this))
    , mAccountsOAuthAppleKid(new QLineEdit(this))
    , mTwitterLogin(new QCheckBox(i18nc("@option:check", "Twitter Login"), this))
    , mTwitterId(new QLineEdit(this))
    , mTwitterSecret(new QLineEdit(this))
    , mTwitterCallbackURL(new QLineEdit(this))
    , mLinkedInLogin(new QCheckBox(i18nc("@option:check", "LinkedIn Login"), this))
    , mLinkedInId(new QLineEdit(this))
    , mLinkedInSecret(new QLineEdit(this))
    , mLinkedInCallbackURL(new QLineEdit(this))
    , mProxyHost(new QLineEdit(this))
    , mProxyServices(new QLineEdit(this))
    , mMeteorLogin(new QCheckBox(i18nc("@option:check", "Meteor Login"), this))
    , mMeteorId(new QLineEdit(this))
    , mMeteorSecret(new QLineEdit(this))
    , mMeteorCallbackURL(new QLineEdit(this))
    , mFacebookLogin(new QCheckBox(i18nc("@option:check", "Facebook Login"), this))
    , mFacebookId(new QLineEdit(this))
    , mFacebookSecret(new QLineEdit(this))
    , mFacebookCallbackURL(new QLineEdit(this))
    , mGithubLogin(new QCheckBox(i18nc("@option:check", "Github Login"), this))
    , mGithubId(new QLineEdit(this))
    , mGithubSecret(new QLineEdit(this))
    , mGithubCallbackURL(new QLineEdit(this))
    , mTokenpassLogin(new QCheckBox(i18nc("@option:check", "Tokenpass Login"), this))
    , mTokenpassId(new QLineEdit(this))
    , mTokenpassSecret(new QLineEdit(this))
    , mTokenpassCallbackURL(new QLineEdit(this))
    , mTokenpassServerURL(new QLineEdit(this))
    , mGoogleLogin(new QCheckBox(i18nc("@option:check", "Google Login"), this))
    , mGoogleId(new QLineEdit(this))
    , mGoogleSecret(new QLineEdit(this))
    , mGoogleCallbackURL(new QLineEdit(this))
    , mDolphinUrl(new QLineEdit(this))
    , mDolphinEnable(new QCheckBox(i18nc("@option:check", "Enable"), this))
    , mDolphinId(new QLineEdit(this))
    , mDolphinSecret(new QLineEdit(this))
{
    auto appleLabel = createBoldLabel(i18n("Apple"));
    appleLabel->setObjectName(u"appleLabel"_s);
    mMainLayout->addWidget(appleLabel);

    mSignWithApple->setObjectName(u"mSignWithApple"_s);
    mSignWithApple->setToolTip(i18nc("@info:tooltip", "If you want Apple login enabled only on mobile, you can leave all fields empty."));
    addCheckBox(mSignWithApple, u"Accounts_OAuth_Apple"_s);

    mAccountsOAuthAppleId->setObjectName(u"mAccountsOAuthAppleId"_s);
    addLineEdit(i18n("OAuth Apple Id"), mAccountsOAuthAppleId, u"Accounts_OAuth_Apple_id"_s);

    mAccountsOAuthAppleIss->setObjectName(u"mAccountsOAuthAppleIss"_s);
    addLineEdit(i18n("OAuth Apple Iss"), mAccountsOAuthAppleIss, u"Accounts_OAuth_Apple_iss"_s);

    mAccountsOAuthAppleKid->setObjectName(u"AccountsOAuthAppleKid"_s);
    addLineEdit(i18n("OAuth Apple Kid"), mAccountsOAuthAppleKid, u"Accounts_OAuth_Apple_kid"_s);

    auto dolphinLabel = createBoldLabel(i18n("Dolphin"));
    dolphinLabel->setObjectName(u"dolphinLabel"_s);
    mMainLayout->addWidget(dolphinLabel);

    mDolphinUrl->setObjectName(u"mDolphinUrl"_s);
    addLineEdit(i18n("Url"), mDolphinUrl, u"Accounts_OAuth_Dolphin_URL"_s);

    mDolphinEnable->setObjectName(u"mDolphinEnable"_s);
    addCheckBox(mDolphinEnable, u"Accounts_OAuth_Dolphin"_s);

    mDolphinId->setObjectName(u"mDolphinId"_s);
    addLineEdit(i18n("Id"), mDolphinId, u"Accounts_OAuth_Dolphin_id"_s);

    mDolphinSecret->setObjectName(u"mDolphinSecret"_s);
    addLineEdit(i18n("Secret"), mDolphinSecret, u"Accounts_OAuth_Dolphin_secret"_s);
    // TODO

    auto twitterLabel = createBoldLabel(i18n("Twitter"));
    twitterLabel->setObjectName(u"twitterLabel"_s);
    mMainLayout->addWidget(twitterLabel);

    mTwitterLogin->setObjectName(u"mTwitterLogin"_s);
    addCheckBox(mTwitterLogin, u"Accounts_OAuth_Twitter"_s);

    mTwitterId->setObjectName(u"mTwitterId"_s);
    addLineEdit(i18n("Twitter Id"), mTwitterId, u"Accounts_OAuth_Twitter_id"_s);

    mTwitterSecret->setObjectName(u"mTwitterSecret"_s);
    addLineEdit(i18n("Twitter Secret"), mTwitterSecret, u"Accounts_OAuth_Twitter_secret"_s);

    mTwitterCallbackURL->setObjectName(u"mTwitterCallbackURL"_s);
    addLineEdit(i18n("Twitter Secret"), mTwitterCallbackURL, u"Accounts_OAuth_Twitter_callback_url"_s);

    auto linkedInLabel = createBoldLabel(i18n("LinkedIn"));
    linkedInLabel->setObjectName(u"linkedInLabel"_s);
    mMainLayout->addWidget(linkedInLabel);

    mLinkedInLogin->setObjectName(u"mLinkedInLogin"_s);
    addCheckBox(mLinkedInLogin, u"Accounts_OAuth_Linkedin"_s);

    mLinkedInId->setObjectName(u"mLinkedInId"_s);
    addLineEdit(i18n("LinkedIn Id"), mLinkedInId, u"Accounts_OAuth_Linkedin_id"_s);

    mLinkedInSecret->setObjectName(u"mLinkedInSecret"_s);
    addLineEdit(i18n("LinkedIn Secret"), mLinkedInSecret, u"Accounts_OAuth_Linkedin_secret"_s);

    mLinkedInCallbackURL->setObjectName(u"mLinkedInCallbackURL"_s);
    addLineEdit(i18n("LinkedIn Secret"), mLinkedInCallbackURL, u"Accounts_OAuth_Linkedin_callback_url"_s);

    auto proxyLabel = createBoldLabel(i18n("Proxy"));
    proxyLabel->setObjectName(u"proxyLabel"_s);
    mMainLayout->addWidget(proxyLabel);

    mProxyHost->setObjectName(u"mProxyHost"_s);
    addLineEdit(i18n("Proxy Host"), mProxyHost, u"Accounts_OAuth_Proxy_host"_s);

    mProxyServices->setObjectName(u"mProxyServices"_s);
    addLineEdit(i18n("Proxy Services"), mProxyServices, u"Accounts_OAuth_Proxy_services"_s);

    // Meteor
    auto meteorLabel = createBoldLabel(i18n("Meteor"));
    meteorLabel->setObjectName(u"meteorLabel"_s);
    mMainLayout->addWidget(meteorLabel);

    mMeteorLogin->setObjectName(u"mMeteorLogin"_s);
    addCheckBox(mMeteorLogin, u"Accounts_OAuth_Meteor"_s);

    mMeteorId->setObjectName(u"mMeteorId"_s);
    addLineEdit(i18n("Meteor Id"), mMeteorId, u"Accounts_OAuth_Meteor_id"_s);

    mMeteorSecret->setObjectName(u"mMeteorSecret"_s);
    addLineEdit(i18n("Meteor Secret"), mMeteorSecret, u"Accounts_OAuth_Meteor_secret"_s);

    mMeteorCallbackURL->setObjectName(u"mMeteorCallbackURL"_s);
    addLineEdit(i18n("Meteor Secret"), mMeteorCallbackURL, u"Accounts_OAuth_Meteor_callback_url"_s);

    // Facebook
    auto facebookLabel = createBoldLabel(i18n("Facebook"));
    facebookLabel->setObjectName(u"facebookLabel"_s);
    mMainLayout->addWidget(facebookLabel);

    mFacebookLogin->setObjectName(u"mFacebookLogin"_s);
    addCheckBox(mFacebookLogin, u"Accounts_OAuth_Facebook"_s);

    mFacebookId->setObjectName(u"mFacebookId"_s);
    addLineEdit(i18n("Facebook Id"), mFacebookId, u"Accounts_OAuth_Facebook_id"_s);

    mFacebookSecret->setObjectName(u"mFacebookSecret"_s);
    addLineEdit(i18n("Facebook Secret"), mFacebookSecret, u"Accounts_OAuth_Facebook_secret"_s);

    mFacebookCallbackURL->setObjectName(u"mFacebookCallbackURL"_s);
    addLineEdit(i18n("Facebook Secret"), mFacebookCallbackURL, u"Accounts_OAuth_Facebook_callback_url"_s);

    // Github
    auto githubLabel = createBoldLabel(i18n("Github"));
    githubLabel->setObjectName(u"githubLabel"_s);
    mMainLayout->addWidget(githubLabel);

    mGithubLogin->setObjectName(u"mGithubLogin"_s);
    addCheckBox(mGithubLogin, u"Accounts_OAuth_Github"_s);

    mGithubId->setObjectName(u"mGithubId"_s);
    addLineEdit(i18n("Github Id"), mGithubId, u"Accounts_OAuth_Github_id"_s);

    mGithubSecret->setObjectName(u"mGithubSecret"_s);
    addLineEdit(i18n("Github Secret"), mGithubSecret, u"Accounts_OAuth_Github_secret"_s);

    mGithubCallbackURL->setObjectName(u"mGithubCallbackURL"_s);
    addLineEdit(i18n("Github Secret"), mGithubCallbackURL, u"Accounts_OAuth_Github_callback_url"_s);

    // Tokenpass
    auto tokenpassLabel = createBoldLabel(i18n("Tokenpass"));
    tokenpassLabel->setObjectName(u"tokenpassLabel"_s);
    mMainLayout->addWidget(tokenpassLabel);

    mTokenpassLogin->setObjectName(u"mTokenpassLogin"_s);
    addCheckBox(mTokenpassLogin, u"Accounts_OAuth_Tokenpass"_s);

    mTokenpassServerURL->setObjectName(u"mTokenpassServerURL"_s);
    mTokenpassServerURL->setToolTip(i18nc("@info:tooltip", "Example: https://domain.com (excluding trailing slash)"));
    addLineEdit(i18n("Tokenpass Server URL"), mTokenpassServerURL, u"API_Tokenpass_URL"_s);

    mTokenpassId->setObjectName(u"mTokenpassId"_s);
    addLineEdit(i18n("Tokenpass Id"), mTokenpassId, u"Accounts_OAuth_Tokenpass_id"_s);

    mTokenpassSecret->setObjectName(u"mTokenpassSecret"_s);
    addLineEdit(i18n("Tokenpass Secret"), mTokenpassSecret, u"Accounts_OAuth_Tokenpass_secret"_s);

    mTokenpassCallbackURL->setObjectName(u"mTokenpassCallbackURL"_s);
    addLineEdit(i18n("Tokenpass Secret"), mTokenpassCallbackURL, u"Accounts_OAuth_Tokenpass_callback_url"_s);

    // Google
    auto googleLabel = createBoldLabel(i18n("Google"));
    googleLabel->setObjectName(u"googleLabel"_s);
    mMainLayout->addWidget(googleLabel);

    mGoogleLogin->setObjectName(u"mGoogleLogin"_s);
    addCheckBox(mGoogleLogin, u"Accounts_OAuth_Google"_s);

    mGoogleId->setObjectName(u"mGoogleId"_s);
    addLineEdit(i18n("Google Id"), mGoogleId, u"Accounts_OAuth_Google_id"_s);

    mGoogleSecret->setObjectName(u"mGoogleSecret"_s);
    addLineEdit(i18n("Google Secret"), mGoogleSecret, u"Accounts_OAuth_Google_secret"_s);

    mGoogleCallbackURL->setObjectName(u"mGoogleCallbackURL"_s);
    addLineEdit(i18n("Google Secret"), mGoogleCallbackURL, u"Accounts_OAuth_Google_callback_url"_s);
}

OauthSettingsWidget::~OauthSettingsWidget() = default;

void OauthSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mSignWithApple, mapSettings, false);
    initializeWidget(mAccountsOAuthAppleId, mapSettings, QString());
    initializeWidget(mAccountsOAuthAppleIss, mapSettings, QString());
    initializeWidget(mAccountsOAuthAppleKid, mapSettings, QString());
    initializeWidget(mTwitterLogin, mapSettings, false);
    initializeWidget(mTwitterId, mapSettings, QString());
    initializeWidget(mTwitterSecret, mapSettings, QString());
    initializeWidget(mTwitterCallbackURL, mapSettings, urlFromRelativePath(u"_oauth/twitter"_s));

    initializeWidget(mLinkedInLogin, mapSettings, false);
    initializeWidget(mLinkedInId, mapSettings, QString());
    initializeWidget(mLinkedInSecret, mapSettings, QString());
    initializeWidget(mLinkedInCallbackURL, mapSettings, urlFromRelativePath(u"_oauth/linkedin"_s));

    initializeWidget(mProxyHost, mapSettings, u"https://oauth-proxy.rocket.chat"_s);
    initializeWidget(mProxyServices, mapSettings, QString());

    initializeWidget(mMeteorLogin, mapSettings, false);
    initializeWidget(mMeteorId, mapSettings, QString());
    initializeWidget(mMeteorSecret, mapSettings, QString());
    initializeWidget(mMeteorCallbackURL, mapSettings, urlFromRelativePath(u"_oauth/meteor"_s));

    initializeWidget(mFacebookLogin, mapSettings, false);
    initializeWidget(mFacebookId, mapSettings, QString());
    initializeWidget(mFacebookSecret, mapSettings, QString());
    initializeWidget(mFacebookCallbackURL, mapSettings, urlFromRelativePath(u"_oauth/facebook"_s));

    initializeWidget(mGithubLogin, mapSettings, false);
    initializeWidget(mGithubId, mapSettings, QString());
    initializeWidget(mGithubSecret, mapSettings, QString());
    initializeWidget(mGithubCallbackURL, mapSettings, urlFromRelativePath(u"_oauth/github"_s));

    initializeWidget(mTokenpassLogin, mapSettings, false);
    initializeWidget(mTokenpassId, mapSettings, QString());
    initializeWidget(mTokenpassSecret, mapSettings, QString());
    initializeWidget(mTokenpassCallbackURL, mapSettings, urlFromRelativePath(u"_oauth/tokenpass"_s));
    initializeWidget(mTokenpassServerURL, mapSettings, QString());

    initializeWidget(mGoogleLogin, mapSettings, false);
    initializeWidget(mGoogleId, mapSettings, QString());
    initializeWidget(mGoogleSecret, mapSettings, QString());
    initializeWidget(mGoogleCallbackURL, mapSettings, urlFromRelativePath(u"_oauth/google"_s));

    initializeWidget(mDolphinUrl, mapSettings, QString());
    initializeWidget(mDolphinEnable, mapSettings, false);
    initializeWidget(mDolphinId, mapSettings, QString());
    initializeWidget(mDolphinSecret, mapSettings, QString());
}

#include "moc_oauthsettingswidget.cpp"
