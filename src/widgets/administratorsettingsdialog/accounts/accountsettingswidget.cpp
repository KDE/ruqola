/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>

AccountSettingsWidget::AccountSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mAllowChangeName(new QCheckBox(i18n("Allow Name Change"), this))
    , mAllowChangeUserProfile(new QCheckBox(i18n("Allow User Profile Change"), this))
    , mAllowChangeUserAvatar(new QCheckBox(i18n("Allow User Avatar Change"), this))
    , mAllowChangeEmail(new QCheckBox(i18n("Allow Email Change"), this))
    , mAllowChangePassword(new QCheckBox(i18n("Allow Password Change"), this))
    , mAllowChangeEmailNotifications(new QCheckBox(i18n("Allow Email Notifications"), this))
    , mAllowUsersToDeleteOwnAccount(new QCheckBox(i18n("Allow Users to Delete Own Account"), this))
    , mAllowAnonymousWrite(new QCheckBox(i18n("Allow Anonymous Write"), this))
    , mAllowAnonymousRead(new QCheckBox(i18n("Allow Anonymous Read"), this))
    , mAllowUsersDeleteOwnAccount(new QCheckBox(i18n("Allow Users to Delete Own Account"), this))
    , mAllowPasswordChangeOauthUsers(new QCheckBox(i18n("Allow Password Change for OAuth Users"), this))
    , mLoginExpirationInDays(new QSpinBox(this))
    , mAllowInvisibleStatusOption(new QCheckBox(i18n("Allow Invisible status option"), this))
    , mForgetUserSessionWindowClose(new QCheckBox(i18n("Forget User Session on Window Close"), this))
{
    mAllowChangeName->setObjectName(QStringLiteral("mAllowChangeName"));
    mMainLayout->addWidget(mAllowChangeName);
    connectCheckBox(mAllowChangeName, QStringLiteral("Accounts_AllowRealNameChange"));

    mAllowChangeUserProfile->setObjectName(QStringLiteral("mAllowChangeUserProfile"));
    mMainLayout->addWidget(mAllowChangeUserProfile);
    connectCheckBox(mAllowChangeUserProfile, QStringLiteral("Accounts_AllowUserProfileChange"));

    mAllowChangeUserAvatar->setObjectName(QStringLiteral("mAllowChangeUserAvatar"));
    mMainLayout->addWidget(mAllowChangeUserAvatar);
    connectCheckBox(mAllowChangeUserAvatar, QStringLiteral("Accounts_AllowUserAvatarChange"));

    mAllowChangeEmail->setObjectName(QStringLiteral("mAllowChangeEmail"));
    mMainLayout->addWidget(mAllowChangeEmail);
    connectCheckBox(mAllowChangeEmail, QStringLiteral("Accounts_AllowEmailChange"));

    mAllowChangePassword->setObjectName(QStringLiteral("mAllowChangePassword"));
    mMainLayout->addWidget(mAllowChangePassword);
    connectCheckBox(mAllowChangePassword, QStringLiteral("Accounts_AllowPasswordChange"));

    mAllowChangeEmailNotifications->setObjectName(QStringLiteral("mAllowChangeEmailNotifications"));
    mMainLayout->addWidget(mAllowChangeEmailNotifications);
    connectCheckBox(mAllowChangeEmailNotifications, QStringLiteral("Accounts_AllowEmailNotifications"));

    mAllowUsersToDeleteOwnAccount->setObjectName(QStringLiteral("mAllowUsersToDeleteOwnAccount"));
    mMainLayout->addWidget(mAllowUsersToDeleteOwnAccount);
    connectCheckBox(mAllowUsersToDeleteOwnAccount, QStringLiteral("Accounts_AllowDeleteOwnAccount"));

    mAllowAnonymousWrite->setObjectName(QStringLiteral("mAllowAnonymousWrite"));
    mMainLayout->addWidget(mAllowAnonymousWrite);
    connectCheckBox(mAllowAnonymousWrite, QStringLiteral("Accounts_AllowAnonymousWrite"));

    mAllowAnonymousRead->setObjectName(QStringLiteral("mAllowAnonymousRead"));
    mMainLayout->addWidget(mAllowAnonymousRead);
    connectCheckBox(mAllowAnonymousRead, QStringLiteral("Accounts_AllowAnonymousRead"));

    mAllowUsersDeleteOwnAccount->setObjectName(QStringLiteral("mAllowUsersDeleteOwnAccount"));
    mMainLayout->addWidget(mAllowUsersDeleteOwnAccount);
    connectCheckBox(mAllowUsersDeleteOwnAccount, QStringLiteral("Accounts_AllowDeleteOwnAccount"));

    mAllowPasswordChangeOauthUsers->setObjectName(QStringLiteral("mAllowPasswordChangeOauthUsers"));
    mMainLayout->addWidget(mAllowPasswordChangeOauthUsers);
    connectCheckBox(mAllowPasswordChangeOauthUsers, QStringLiteral("Accounts_AllowPasswordChangeForOAuthUsers"));

    mLoginExpirationInDays->setObjectName(QStringLiteral("mLoginExpirationInDays"));
    addSpinbox(i18n("Login Expiration in Days"), mLoginExpirationInDays, QStringLiteral("Accounts_LoginExpiration"));

    mAllowInvisibleStatusOption->setObjectName(QStringLiteral("mAllowInvisibleStatusOption"));
    mMainLayout->addWidget(mAllowInvisibleStatusOption);
    connectCheckBox(mAllowInvisibleStatusOption, QStringLiteral("Accounts_AllowInvisibleStatusOption"));

    mForgetUserSessionWindowClose->setObjectName(QStringLiteral("mForgetUserSessionWindowClose"));
    mMainLayout->addWidget(mForgetUserSessionWindowClose);
    connectCheckBox(mForgetUserSessionWindowClose, QStringLiteral("Accounts_ForgetUserSessionOnWindowClose"));

    auto failedLoginAttemptsLabel = createBoldLabel(i18n("Failed Login Attempts"));
    failedLoginAttemptsLabel->setObjectName(QStringLiteral("failedLoginAttemptsLabel"));
    mMainLayout->addWidget(failedLoginAttemptsLabel);
}

AccountSettingsWidget::~AccountSettingsWidget() = default;

void AccountSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mAllowChangeName, mapSettings, true);
    initializeWidget(mAllowChangeUserProfile, mapSettings, true);
    initializeWidget(mAllowChangeUserAvatar, mapSettings, true);
    initializeWidget(mAllowChangeEmail, mapSettings, true);
    initializeWidget(mAllowChangePassword, mapSettings, true);
    initializeWidget(mAllowChangeEmailNotifications, mapSettings, true);
    initializeWidget(mAllowUsersToDeleteOwnAccount, mapSettings, true);
    initializeWidget(mAllowAnonymousWrite, mapSettings, false);
    initializeWidget(mAllowAnonymousRead, mapSettings, false);
    initializeWidget(mAllowUsersDeleteOwnAccount, mapSettings, false);
    initializeWidget(mAllowPasswordChangeOauthUsers, mapSettings, false);
    initializeWidget(mLoginExpirationInDays, mapSettings, 90);
    initializeWidget(mAllowInvisibleStatusOption, mapSettings, true);
    initializeWidget(mForgetUserSessionWindowClose, mapSettings, false);
}
