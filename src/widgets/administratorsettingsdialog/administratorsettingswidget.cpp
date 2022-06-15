/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorsettingswidget.h"
#include "accounts/accountsettingswidget.h"
#include "administratorsettingsdialog/ratelimiter/ratelimiterwidget.h"
#include "encryption/encryptionsettingswidget.h"
#include "fileupload/fileuploadsettingswidget.h"
#include "general/generalsettingswidget.h"
#include "message/messagesettingswidget.h"
#include "retentionpolicy/retentionpolicysettingswidget.h"
#include "rocketchataccount.h"
#include "rocketchatbackend.h"

#include <QTabWidget>
#include <QVBoxLayout>

#include <KLocalizedString>

AdministratorSettingsWidget::AdministratorSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mTabWidget(new QTabWidget(this))
    , mAccountSettingsWidget(new AccountSettingsWidget(account, this))
    , mEncryptionSettingsWidget(new EncryptionSettingsWidget(account, this))
    , mMessageSettingsWidget(new MessageSettingsWidget(account, this))
    , mUploadFileSettingsWidget(new FileUploadSettingsWidget(account, this))
    , mRetentionPolicySettingsWidget(new RetentionPolicySettingsWidget(account, this))
    , mGeneralSettingsWidget(new GeneralSettingsWidget(account, this))
    , mRateLimiterWidget(new RateLimiterWidget(account, this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mTabWidget->setObjectName(QStringLiteral("mTabWidget"));
    mainLayout->addWidget(mTabWidget);

    mAccountSettingsWidget->setObjectName(QStringLiteral("mAccountSettingsWidget"));
    mEncryptionSettingsWidget->setObjectName(QStringLiteral("mEncryptionSettingsWidget"));
    mMessageSettingsWidget->setObjectName(QStringLiteral("mMessageSettingsWidget"));
    mUploadFileSettingsWidget->setObjectName(QStringLiteral("mUploadFileSettingsWidget"));
    mRetentionPolicySettingsWidget->setObjectName(QStringLiteral("mRetentionPolicySettingsWidget"));
    mGeneralSettingsWidget->setObjectName(QStringLiteral("mGeneralSettingsWidget"));
    mRateLimiterWidget->setObjectName(QStringLiteral("mRateLimiterWidget"));
    mTabWidget->addTab(mAccountSettingsWidget, i18n("Accounts"));
    mTabWidget->addTab(mEncryptionSettingsWidget, i18n("Encryption"));
    mTabWidget->addTab(mMessageSettingsWidget, i18n("Message"));
    mTabWidget->addTab(mUploadFileSettingsWidget, i18n("File Upload"));
    mTabWidget->addTab(mRetentionPolicySettingsWidget, i18n("Retention Policy"));
    mTabWidget->addTab(mGeneralSettingsWidget, i18n("General"));
    mTabWidget->addTab(mRateLimiterWidget, i18n("Rate Limiter"));
    if (mRocketChatAccount) {
        connect(mRocketChatAccount, &RocketChatAccount::publicSettingLoaded, this, &AdministratorSettingsWidget::initialize);
    }
}

AdministratorSettingsWidget::~AdministratorSettingsWidget() = default;

void AdministratorSettingsWidget::loadSettings()
{
    mRocketChatAccount->rocketChatBackend()->loadPublicSettings();
}

void AdministratorSettingsWidget::initialize(const QJsonObject &obj)
{
    QJsonArray configs = obj.value(QLatin1String("result")).toArray();
    QMap<QString, QVariant> mapSettings;
    for (QJsonValueRef currentConfig : configs) {
        const QJsonObject currentConfObject = currentConfig.toObject();
        const QString id = currentConfObject[QStringLiteral("_id")].toString();
        const QVariant value = currentConfObject[QStringLiteral("value")].toVariant();
        mapSettings.insert(id, value);
    }

    mAccountSettingsWidget->initialize(mapSettings);
    mEncryptionSettingsWidget->initialize(mapSettings);
    mMessageSettingsWidget->initialize(mapSettings);
    mUploadFileSettingsWidget->initialize(mapSettings);
    mRetentionPolicySettingsWidget->initialize(mapSettings);
    mGeneralSettingsWidget->initialize(mapSettings);
    mRateLimiterWidget->initialize(mapSettings);
}

void AdministratorSettingsWidget::updatePage()
{
    mAccountSettingsWidget->setEnabled(true);
    mEncryptionSettingsWidget->setEnabled(true);
    mMessageSettingsWidget->setEnabled(true);
    mUploadFileSettingsWidget->setEnabled(true);
    mRetentionPolicySettingsWidget->setEnabled(true);
    mRateLimiterWidget->setEnabled(true);
}
