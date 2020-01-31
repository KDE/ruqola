/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "createnewaccountdialog.h"
#include "createnewaccountwidget.h"
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <KConfigGroup>
#include <KSharedConfig>

namespace {
static const char myConfigGroupName[] = "CreateNewAccountDialog";
}

CreateNewAccountDialog::CreateNewAccountDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Add Account"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mNewAccountWidget = new CreateNewAccountWidget(this);
    mNewAccountWidget->setObjectName(QStringLiteral("mNewAccountWidget"));
    mainLayout->addWidget(mNewAccountWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &CreateNewAccountDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &CreateNewAccountDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
    mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    mOkButton->setEnabled(false);
    connect(mNewAccountWidget, &CreateNewAccountWidget::updateOkButton, this, [this](bool state) {
        mOkButton->setEnabled(state);
    });
}

CreateNewAccountDialog::~CreateNewAccountDialog()
{
    writeConfig();
}

CreateNewAccountDialog::AccountInfo CreateNewAccountDialog::accountInfo() const
{
    AccountInfo info;
    info.accountName = mNewAccountWidget->accountName();
    info.userName = mNewAccountWidget->userName();
    info.serverName = mNewAccountWidget->serverName();
    return info;
}

void CreateNewAccountDialog::setAccountInfo(const CreateNewAccountDialog::AccountInfo &info)
{
    mNewAccountWidget->setAccountName(info.accountName);
    //mNewAccountWidget->(info.accountName);
    //mNewAccountWidget->setAccountName(info.accountName);
}

void CreateNewAccountDialog::setAccountName(const QString &name)
{
    mNewAccountWidget->setAccountName(name);
    setWindowTitle(i18nc("@title:window", "Modify Account"));
}

void CreateNewAccountDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void CreateNewAccountDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}
