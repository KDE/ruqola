/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorsettingswidget.h"
#include "accounts/accountsettingswidget.h"
#include "encryption/encryptionsettingswidget.h"
#include "rocketchataccount.h"

#include <QTabWidget>
#include <QVBoxLayout>

#include <KLocalizedString>

AdministratorSettingsWidget::AdministratorSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mTabWidget(new QTabWidget(this))
    , mAccountSettingsWidget(new AccountSettingsWidget(account, this))
    , mEncryptionSettingsWidget(new EncryptionSettingsWidget(account, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mTabWidget->setObjectName(QStringLiteral("mTabWidget"));
    mainLayout->addWidget(mTabWidget);

    mAccountSettingsWidget->setObjectName(QStringLiteral("mAccountSettingsWidget"));
    mEncryptionSettingsWidget->setObjectName(QStringLiteral("mEncryptionSettingsWidget"));
    mTabWidget->addTab(mAccountSettingsWidget, i18n("Accounts"));
    // TODO load settings
}

AdministratorSettingsWidget::~AdministratorSettingsWidget()
{
}

void AdministratorSettingsWidget::initialize()
{
    mAccountSettingsWidget->initialize();
}
