/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorsettingswidget.h"
#include "accounts/accountsettingswidget.h"
#include "encryption/encryptionsettingswidget.h"
#include "fileupload/fileuploadsettingswidget.h"
#include "general/generalsettingswidget.h"
#include "message/messagesettingswidget.h"
#include "retentionpolicy/retentionpolicysettingswidget.h"
#include "rocketchataccount.h"

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
    mTabWidget->addTab(mAccountSettingsWidget, i18n("Accounts"));
    mTabWidget->addTab(mEncryptionSettingsWidget, i18n("Encryption"));
    mTabWidget->addTab(mMessageSettingsWidget, i18n("Message"));
    mTabWidget->addTab(mUploadFileSettingsWidget, i18n("File Upload"));
    mTabWidget->addTab(mRetentionPolicySettingsWidget, i18n("Retention Policy"));
    mTabWidget->addTab(mGeneralSettingsWidget, i18n("General"));
}

AdministratorSettingsWidget::~AdministratorSettingsWidget() = default;

void AdministratorSettingsWidget::initialize()
{
    // TODO
    const QMap<QString, QVariant> mapSettings;
    mAccountSettingsWidget->initialize(mapSettings);
}

void AdministratorSettingsWidget::updatePage()
{
    mAccountSettingsWidget->setEnabled(true);
    mEncryptionSettingsWidget->setEnabled(true);
    mMessageSettingsWidget->setEnabled(true);
    mUploadFileSettingsWidget->setEnabled(true);
    mRetentionPolicySettingsWidget->setEnabled(true);
}
