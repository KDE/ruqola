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

AdministratorDialog::AdministratorDialog(QWidget *parent)
    : QDialog(parent)
    , mAdministratorWidget(new AdministratorWidget(this))
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
