/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorsettingsdialog.h"
#include "administratorsettingswidget.h"

#include "rocketchataccount.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
const char myAdministratorSettingsDialogGroupName[] = "AdministratorSettingsDialog";
}

AdministratorSettingsDialog::AdministratorSettingsDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mAdministratorSettingsWidget(new AdministratorSettingsWidget(account, this))
{
    // Use "account-name" for autotest only when account is empty
    setWindowTitle(i18nc("@title:window", "Administrator Server Settings - %1", account ? account->accountName() : QStringLiteral("account-name")));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mAdministratorSettingsWidget->setObjectName(QStringLiteral("mAdministratorSettingsWidget"));
    mainLayout->addWidget(mAdministratorSettingsWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AdministratorSettingsDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorSettingsDialog::accept);

    readConfig();
}

AdministratorSettingsDialog::~AdministratorSettingsDialog()
{
    writeConfig();
}

void AdministratorSettingsDialog::loadSettings()
{
    mAdministratorSettingsWidget->loadSettings();
}

void AdministratorSettingsDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), myAdministratorSettingsDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void AdministratorSettingsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myAdministratorSettingsDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}
