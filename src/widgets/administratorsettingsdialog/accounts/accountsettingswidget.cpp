/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountsettingswidget.h"
#include "connection.h"
#include "dialogs/confirmpassworddialog.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "settings/updateadminsettingsjob.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>

AccountSettingsWidget::AccountSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mAllowChangeName(new QCheckBox(i18n("Allow Name Change"), this))
    , mAllowChangeUserProfile(new QCheckBox(i18n("Allow User Profile Change"), this))
    , mAllowChangeUserAvatar(new QCheckBox(i18n("Allow User Avatar Change"), this))
    , mAllowChangeEmail(new QCheckBox(i18n("Allow Email Change"), this))
    , mAllowChangePassword(new QCheckBox(i18n("Allow Password Change"), this))
    , mAllowChangeEmailNotifications(new QCheckBox(i18n("Allow Email Notifications"), this))
    , mAllowUsersToDeleteOwnAccount(new QCheckBox(i18n("Allow Users to Delete Own Account"), this))
    , mAllowAnonymousWrite(new QCheckBox(i18n("Allow Anonymous Write"), this))
    , mAccount(account)
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
}

void AccountSettingsWidget::initialize()
{
    // TODO
}

AccountSettingsWidget::~AccountSettingsWidget() = default;

void AccountSettingsWidget::updateSettings(const QString &settingName, bool value)
{
    if (mAccount) {
        QString password;
        QPointer<ConfirmPasswordDialog> dialog(new ConfirmPasswordDialog(this));
        if (dialog->exec()) {
            password = dialog->password();
        }
        delete dialog;

        auto job = new RocketChatRestApi::UpdateAdminSettingsJob(this);
        RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo info;
        info.settingsValue = value;
        info.settingName = settingName;
        job->setUpdateAdminSettingsInfo(info);
        job->setAuthMethod(QStringLiteral("password"));
        job->setAuthCode(QString::fromLatin1(Utils::convertSha256Password(password)));
        mAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::UpdateAdminSettingsJob::updateAdminSettingsDone, this, &AccountSettingsWidget::slotAdminSettingsDone);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start UpdateAdminSettingsJob job";
        }
    }
}

void AccountSettingsWidget::slotAdminSettingsDone(const QJsonObject &obj)
{
    qDebug() << "AccountSettingsWidget::slotAdminSettingsDone " << obj;
}
