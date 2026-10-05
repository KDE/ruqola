/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorsettingsdialog.h"

#include "administratorsettingswidget.h"

#include "rocketchataccount.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

using namespace Qt::Literals::StringLiterals;

namespace
{
const char myAdministratorSettingsDialogGroupName[] = "AdministratorSettingsDialog";
}

AdministratorSettingsDialog::AdministratorSettingsDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mAdministratorSettingsWidget(new AdministratorSettingsWidget(account, this))
{
    // Use "account-name" for autotest only when account is empty
    setWindowTitle(i18nc("@title:window", "Administrator Server Settings - %1", account ? account->accountName() : u"account-name"_s));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mAdministratorSettingsWidget->setObjectName(u"mAdministratorSettingsWidget"_s);
    mainLayout->addWidget(mAdministratorSettingsWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AdministratorSettingsDialog::reject);

    readConfig();
}

AdministratorSettingsDialog::~AdministratorSettingsDialog() = default;

void AdministratorSettingsDialog::loadSettings()
{
    mAdministratorSettingsWidget->loadSettings();
}

void AdministratorSettingsDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myAdministratorSettingsDialogGroupName), QSize(400, 300));
}

#include "moc_administratorsettingsdialog.cpp"
