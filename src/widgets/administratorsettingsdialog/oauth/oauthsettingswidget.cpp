/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

OauthSettingsWidget::OauthSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mSignWithApple(new QCheckBox(i18n("Sign in with Apple"), this))
    , mAccountsOAuthAppleId(new QLineEdit(this))
    , mAccountsOAuthAppleIss(new QLineEdit(this))
    , mAccountsOAuthAppleKid(new QLineEdit(this))
    , mTwitterLogin(new QCheckBox(i18n("Twitter Login"), this))
    , mTwitterId(new QLineEdit(this))
    , mTwitterSecret(new QLineEdit(this))
    , mTwitterCallbackURL(new QLineEdit(this))
{
    auto appleLabel = createBoldLabel(i18n("Apple"));
    appleLabel->setObjectName(QStringLiteral("appleLabel"));
    mMainLayout->addWidget(appleLabel);

    mSignWithApple->setObjectName(QStringLiteral("mSignWithApple"));
    mMainLayout->addWidget(mSignWithApple);
    mSignWithApple->setToolTip(i18n("If you want Apple login enabled only on mobile, you can leave all fields empty."));
    connectCheckBox(mSignWithApple, QStringLiteral("Accounts_OAuth_Apple"));

    mAccountsOAuthAppleId->setObjectName(QStringLiteral("mAccountsOAuthAppleId"));
    addLineEdit(i18n("OAuth Apple Id"), mAccountsOAuthAppleId, QStringLiteral("Accounts_OAuth_Apple_id"));

    mAccountsOAuthAppleIss->setObjectName(QStringLiteral("mAccountsOAuthAppleIss"));
    addLineEdit(i18n("OAuth Apple Iss"), mAccountsOAuthAppleIss, QStringLiteral("Accounts_OAuth_Apple_iss"));

    mAccountsOAuthAppleKid->setObjectName(QStringLiteral("AccountsOAuthAppleKid"));
    addLineEdit(i18n("OAuth Apple Kid"), mAccountsOAuthAppleKid, QStringLiteral("Accounts_OAuth_Apple_kid"));

    auto dolphinLabel = createBoldLabel(i18n("Dolphin"));
    dolphinLabel->setObjectName(QStringLiteral("dolphinLabel"));
    mMainLayout->addWidget(dolphinLabel);
    // TODO

    auto twitterLabel = createBoldLabel(i18n("Twitter"));
    twitterLabel->setObjectName(QStringLiteral("twitterLabel"));
    mMainLayout->addWidget(twitterLabel);

    mTwitterLogin->setObjectName(QStringLiteral("mTwitterLogin"));
    mMainLayout->addWidget(mTwitterLogin);
    connectCheckBox(mTwitterLogin, QStringLiteral("Accounts_OAuth_Twitter"));

    mTwitterId->setObjectName(QStringLiteral("mTwitterId"));
    addLineEdit(i18n("Twitter Id"), mTwitterId, QStringLiteral("Accounts_OAuth_Twitter_id"));

    mTwitterSecret->setObjectName(QStringLiteral("mTwitterSecret"));
    addLineEdit(i18n("Twitter Secret"), mTwitterSecret, QStringLiteral("Accounts_OAuth_Twitter_secret"));

    mTwitterCallbackURL->setObjectName(QStringLiteral("mTwitterCallbackURL"));
    addLineEdit(i18n("Twitter Secret"), mTwitterCallbackURL, QStringLiteral("Accounts_OAuth_Twitter_callback_url"), true);
}

OauthSettingsWidget::~OauthSettingsWidget() = default;

void OauthSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mSignWithApple, mapSettings, false);
    initializeWidget(mAccountsOAuthAppleId, mapSettings, QString());
    initializeWidget(mAccountsOAuthAppleIss, mapSettings, QString());
    initializeWidget(mAccountsOAuthAppleKid, mapSettings, QString());
    initializeWidget(mTwitterLogin, mapSettings, false);
    initializeWidget(mTwitterId, mapSettings, QString());
    initializeWidget(mTwitterSecret, mapSettings, QString());
    initializeWidget(mTwitterCallbackURL, mapSettings, urlFromRelativePath(QStringLiteral("_oauth/twitter")));
}
