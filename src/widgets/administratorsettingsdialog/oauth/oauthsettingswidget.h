/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT OauthSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit OauthSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~OauthSettingsWidget() override;

    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QCheckBox *const mSignWithApple;
    QLineEdit *const mAccountsOAuthAppleId;
    QLineEdit *const mAccountsOAuthAppleIss;
    QLineEdit *const mAccountsOAuthAppleKid;
    QCheckBox *const mTwitterLogin;
    QLineEdit *const mTwitterId;
    QLineEdit *const mTwitterSecret;
    QLineEdit *const mTwitterCallbackURL;

    QCheckBox *const mLinkedInLogin;
    QLineEdit *const mLinkedInId;
    QLineEdit *const mLinkedInSecret;
    QLineEdit *const mLinkedInCallbackURL;

    QLineEdit *const mProxyHost;
    QLineEdit *const mProxyServices;

    QCheckBox *const mMeteorLogin;
    QLineEdit *const mMeteorId;
    QLineEdit *const mMeteorSecret;
    QLineEdit *const mMeteorCallbackURL;

    QCheckBox *const mFacebookLogin;
    QLineEdit *const mFacebookId;
    QLineEdit *const mFacebookSecret;
    QLineEdit *const mFacebookCallbackURL;

    QCheckBox *const mGithubLogin;
    QLineEdit *const mGithubId;
    QLineEdit *const mGithubSecret;
    QLineEdit *const mGithubCallbackURL;

    QCheckBox *const mTokenpassLogin;
    QLineEdit *const mTokenpassId;
    QLineEdit *const mTokenpassSecret;
    QLineEdit *const mTokenpassCallbackURL;
    QLineEdit *const mTokenpassServerURL;
};
