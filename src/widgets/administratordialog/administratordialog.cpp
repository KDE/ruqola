/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratordialog.h"
#include "administratorwidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>

namespace
{
const char myAdministratorDialogGroupName[] = "AdministratorDialog";
}

AdministratorDialog::AdministratorDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mAdministratorWidget(new AdministratorWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Administrator"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mAdministratorWidget->setObjectName(QStringLiteral("mAdministratorWidget"));
    mainLayout->addWidget(mAdministratorWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AdministratorDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorDialog::accept);

    readConfig();
}

AdministratorDialog::~AdministratorDialog()
{
    writeConfig();
}

void AdministratorDialog::initialize()
{
    mAdministratorWidget->updateUiFromPermission();
    mAdministratorWidget->initialize();
}

void AdministratorDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myAdministratorDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void AdministratorDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myAdministratorDialogGroupName);
    group.writeEntry("Size", size());
}
