/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "administratorcustomuserstatuscreatedialog.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
namespace
{
static const char myConfigAdministratorCustomUserStatusCreateDialogGroupName[] = "AdministratorCustomUserStatusCreateDialog";
}
AdministratorCustomUserStatusCreateDialog::AdministratorCustomUserStatusCreateDialog(QWidget *parent)
    : QDialog(parent)
    , mCreateWidget(new AdministratorCustomUserStatusCreateWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Create New User Status"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateWidget->setObjectName(QStringLiteral("mCreateWidget"));
    mainLayout->addWidget(mCreateWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AdministratorCustomUserStatusCreateDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorCustomUserStatusCreateDialog::accept);
    mOkButton = button->button(QDialogButtonBox::Ok);
    mOkButton->setEnabled(false);
    connect(mCreateWidget, &AdministratorCustomUserStatusCreateWidget::updateOkButton, mOkButton, &QPushButton::setEnabled);
    readConfig();
}

AdministratorCustomUserStatusCreateDialog::~AdministratorCustomUserStatusCreateDialog()
{
    writeConfig();
}

void AdministratorCustomUserStatusCreateDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigAdministratorCustomUserStatusCreateDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void AdministratorCustomUserStatusCreateDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigAdministratorCustomUserStatusCreateDialogGroupName);
    group.writeEntry("Size", size());
}

AdministratorCustomUserStatusCreateWidget::UserStatusInfo AdministratorCustomUserStatusCreateDialog::userStatusInfo() const
{
    return mCreateWidget->userStatusInfo();
}

void AdministratorCustomUserStatusCreateDialog::setUserStatusInfo(const AdministratorCustomUserStatusCreateWidget::UserStatusInfo &info)
{
    mCreateWidget->setUserStatusInfo(info);
    setWindowTitle(i18nc("@title:window", "Edit User Status"));
}
