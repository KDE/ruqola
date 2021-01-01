/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "administratorcustomuserstatuscreatedialog.h"

#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QPushButton>
namespace {
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
    KConfigGroup group(KSharedConfig::openConfig(), myConfigAdministratorCustomUserStatusCreateDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void AdministratorCustomUserStatusCreateDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigAdministratorCustomUserStatusCreateDialogGroupName);
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
