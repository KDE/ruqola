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

#include "createnewserverdialog.h"
#include "createnewserverwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{
static const char myConfigCreateNewAccountDialogGroupName[] = "CreateNewAccountDialog";
}

CreateNewServerDialog::CreateNewServerDialog(QWidget *parent)
    : QDialog(parent)
    , mNewAccountWidget(new CreateNewServerWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Add Server"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mNewAccountWidget->setObjectName(QStringLiteral("mNewAccountWidget"));
    mainLayout->addWidget(mNewAccountWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &CreateNewServerDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &CreateNewServerDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
    mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    mOkButton->setEnabled(false);
    connect(mNewAccountWidget, &CreateNewServerWidget::updateOkButton, this, [this](bool state) {
        mOkButton->setEnabled(state);
    });
}

CreateNewServerDialog::~CreateNewServerDialog()
{
    writeConfig();
}

AccountManager::AccountManagerInfo CreateNewServerDialog::accountInfo() const
{
    const AccountManager::AccountManagerInfo info = mNewAccountWidget->accountInfo();
    return info;
}

void CreateNewServerDialog::setAccountInfo(const AccountManager::AccountManagerInfo &info)
{
    setWindowTitle(i18nc("@title:window", "Modify Account"));
    mNewAccountWidget->setAccountInfo(info);
}

void CreateNewServerDialog::setExistingAccountName(const QStringList &lst)
{
    mNewAccountWidget->setExistingAccountName(lst);
}

void CreateNewServerDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigCreateNewAccountDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void CreateNewServerDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigCreateNewAccountDialogGroupName);
    group.writeEntry("Size", size());
}
