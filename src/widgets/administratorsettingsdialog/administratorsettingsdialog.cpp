/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorsettingsdialog.h"
// #include "administratorwidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>

namespace
{
const char myAdministratorSettingsDialogGroupName[] = "AdministratorSettingsDialog";
}

AdministratorSettingsDialog::AdministratorSettingsDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
// , mAdministratorWidget(new AdministratorWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Administrator"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    // mAdministratorWidget->setObjectName(QStringLiteral("mAdministratorWidget"));
    // mainLayout->addWidget(mAdministratorWidget);

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

void AdministratorSettingsDialog::initialize()
{
    //    mAdministratorWidget->updateUiFromPermission();
    //    mAdministratorWidget->initialize();
}

void AdministratorSettingsDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myAdministratorSettingsDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void AdministratorSettingsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myAdministratorSettingsDialogGroupName);
    group.writeEntry("Size", size());
}
