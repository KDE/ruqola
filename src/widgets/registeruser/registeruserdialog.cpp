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

#include "registeruserdialog.h"
#include "registeruserwidget.h"
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <KLocalizedString>
#include <KConfigGroup>
#include <KSharedConfig>

namespace {
const char myConfigGroupName[] = "RegisterUserDialog";
}

RegisterUserDialog::RegisterUserDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Register User"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mRegisterUserWidget = new RegisterUserWidget(this);
    mRegisterUserWidget->setObjectName(QStringLiteral("mRegisterUserWidget"));
    mainLayout->addWidget(mRegisterUserWidget);

    QDialogButtonBox *button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &RegisterUserDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &RegisterUserDialog::accept);
    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    connect(mRegisterUserWidget, &RegisterUserWidget::updateOkButton, okButton, &QPushButton::setEnabled);
    okButton->setEnabled(false);
    readConfig();
}

RegisterUserDialog::~RegisterUserDialog()
{
    writeConfig();
}

void RegisterUserDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void RegisterUserDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}

RocketChatRestApi::RegisterUserJob::RegisterUserInfo RegisterUserDialog::registerUserInfo() const
{
    return mRegisterUserWidget->registerUserInfo();
}
