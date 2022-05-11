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
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mAllowChangeName->setObjectName(QStringLiteral("mAllowChangeName"));
    mainLayout->addWidget(mAllowChangeName);
    connect(mAllowChangeName, &QCheckBox::clicked, this, [this](bool checked) {
        updateSettings(QStringLiteral("Accounts_AllowRealNameChange"), checked);
    });

    mAllowChangeUserProfile->setObjectName(QStringLiteral("mAllowChangeUserProfile"));
    mainLayout->addWidget(mAllowChangeUserProfile);
    connect(mAllowChangeUserProfile, &QCheckBox::clicked, this, [this](bool checked) {
        updateSettings(QStringLiteral("Accounts_AllowUserProfileChange"), checked);
    });

    mAllowChangeUserAvatar->setObjectName(QStringLiteral("mAllowChangeUserAvatar"));
    mainLayout->addWidget(mAllowChangeUserAvatar);
    connect(mAllowChangeUserAvatar, &QCheckBox::clicked, this, [this](bool checked) {
        updateSettings(QStringLiteral("Accounts_AllowUserAvatarChange"), checked);
    });

    mAllowChangeEmail->setObjectName(QStringLiteral("mAllowChangeEmail"));
    mainLayout->addWidget(mAllowChangeEmail);
    connect(mAllowChangeEmail, &QCheckBox::clicked, this, [this](bool checked) {
        updateSettings(QStringLiteral("Accounts_AllowEmailChange"), checked);
    });

    mAllowChangePassword->setObjectName(QStringLiteral("mAllowChangePassword"));
    mainLayout->addWidget(mAllowChangePassword);
    connect(mAllowChangePassword, &QCheckBox::clicked, this, [this](bool checked) {
        updateSettings(QStringLiteral("Accounts_AllowPasswordChange"), checked);
    });

    mAllowChangeEmailNotifications->setObjectName(QStringLiteral("mAllowChangeEmailNotifications"));
    mainLayout->addWidget(mAllowChangeEmailNotifications);
    connect(mAllowChangeEmailNotifications, &QCheckBox::clicked, this, [this](bool checked) {
        updateSettings(QStringLiteral("Accounts_AllowEmailNotifications"), checked);
    });

    mAllowUsersToDeleteOwnAccount->setObjectName(QStringLiteral("mAllowUsersToDeleteOwnAccount"));
    mainLayout->addWidget(mAllowUsersToDeleteOwnAccount);
    connect(mAllowUsersToDeleteOwnAccount, &QCheckBox::clicked, this, [this](bool checked) {
        updateSettings(QStringLiteral("Accounts_AllowDeleteOwnAccount"), checked);
    });

    mAllowAnonymousWrite->setObjectName(QStringLiteral("mAllowAnonymousWrite"));
    mainLayout->addWidget(mAllowAnonymousWrite);
    connect(mAllowAnonymousWrite, &QCheckBox::clicked, this, [this](bool checked) {
        updateSettings(QStringLiteral("Accounts_AllowAnonymousWrite"), checked);
    });

    mAllowUsersDeleteOwnAccount->setObjectName(QStringLiteral("mAllowUsersDeleteOwnAccount"));
    mainLayout->addWidget(mAllowUsersDeleteOwnAccount);
    connect(mAllowUsersDeleteOwnAccount, &QCheckBox::clicked, this, [this](bool checked) {
        updateSettings(QStringLiteral("Accounts_AllowDeleteOwnAccount"), checked);
    });
}

void AccountSettingsWidget::initialize()
{
    // TODO
}

AccountSettingsWidget::~AccountSettingsWidget() = default;
