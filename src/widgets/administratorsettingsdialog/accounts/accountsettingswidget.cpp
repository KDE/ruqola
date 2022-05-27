/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>

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
    , mAllowUsersDeleteOwnAccount(new QCheckBox(i18n("Allow Users to Delete Own Account"), this))
    , mAllowPasswordChangeOauthUsers(new QCheckBox(i18n("Allow Password Change for OAuth Users"), this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mAllowChangeName->setObjectName(QStringLiteral("mAllowChangeName"));
    mainLayout->addWidget(mAllowChangeName);
    connectCheckBox(mAllowChangeName, QStringLiteral("Accounts_AllowRealNameChange"));

    mAllowChangeUserProfile->setObjectName(QStringLiteral("mAllowChangeUserProfile"));
    mainLayout->addWidget(mAllowChangeUserProfile);
    connectCheckBox(mAllowChangeUserProfile, QStringLiteral("Accounts_AllowUserProfileChange"));

    mAllowChangeUserAvatar->setObjectName(QStringLiteral("mAllowChangeUserAvatar"));
    mainLayout->addWidget(mAllowChangeUserAvatar);
    connectCheckBox(mAllowChangeUserAvatar, QStringLiteral("Accounts_AllowUserAvatarChange"));

    mAllowChangeEmail->setObjectName(QStringLiteral("mAllowChangeEmail"));
    mainLayout->addWidget(mAllowChangeEmail);
    connectCheckBox(mAllowChangeEmail, QStringLiteral("Accounts_AllowEmailChange"));

    mAllowChangePassword->setObjectName(QStringLiteral("mAllowChangePassword"));
    mainLayout->addWidget(mAllowChangePassword);
    connectCheckBox(mAllowChangePassword, QStringLiteral("Accounts_AllowPasswordChange"));

    mAllowChangeEmailNotifications->setObjectName(QStringLiteral("mAllowChangeEmailNotifications"));
    mainLayout->addWidget(mAllowChangeEmailNotifications);
    connectCheckBox(mAllowChangeEmailNotifications, QStringLiteral("Accounts_AllowEmailNotifications"));

    mAllowUsersToDeleteOwnAccount->setObjectName(QStringLiteral("mAllowUsersToDeleteOwnAccount"));
    mainLayout->addWidget(mAllowUsersToDeleteOwnAccount);
    connectCheckBox(mAllowUsersToDeleteOwnAccount, QStringLiteral("Accounts_AllowDeleteOwnAccount"));

    mAllowAnonymousWrite->setObjectName(QStringLiteral("mAllowAnonymousWrite"));
    mainLayout->addWidget(mAllowAnonymousWrite);
    connectCheckBox(mAllowAnonymousWrite, QStringLiteral("Accounts_AllowAnonymousWrite"));

    mAllowUsersDeleteOwnAccount->setObjectName(QStringLiteral("mAllowUsersDeleteOwnAccount"));
    mainLayout->addWidget(mAllowUsersDeleteOwnAccount);
    connectCheckBox(mAllowUsersDeleteOwnAccount, QStringLiteral("Accounts_AllowDeleteOwnAccount"));

    mAllowPasswordChangeOauthUsers->setObjectName(QStringLiteral("mAllowPasswordChangeOauthUsers"));
    mainLayout->addWidget(mAllowPasswordChangeOauthUsers);
    connectCheckBox(mAllowPasswordChangeOauthUsers, QStringLiteral("Accounts_AllowPasswordChangeForOAuthUsers"));
}

AccountSettingsWidget::~AccountSettingsWidget() = default;

void AccountSettingsWidget::initialize()
{
    // TODO
}
