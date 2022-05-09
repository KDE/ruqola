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
