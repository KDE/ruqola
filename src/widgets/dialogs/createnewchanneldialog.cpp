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

#include "createnewchanneldialog.h"
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <KLocalizedString>
#include <KConfigGroup>
#include <KSharedConfig>

namespace {
static const char myConfigGroupName[] = "CreateNewChannelDialog";
}
CreateNewChannelDialog::CreateNewChannelDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Create Channel"));
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateNewChannelWidget = new CreateNewChannelWidget(this);
    mCreateNewChannelWidget->setObjectName(QStringLiteral("mCreateNewChannelWidget"));
    mainLayout->addWidget(mCreateNewChannelWidget);

    auto *button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::accepted, this, &CreateNewChannelDialog::accept);
    connect(button, &QDialogButtonBox::rejected, this, &CreateNewChannelDialog::reject);
    readConfig();
    mOkButton = button->button(QDialogButtonBox::Ok);
    mOkButton->setEnabled(false);
    connect(mCreateNewChannelWidget, &CreateNewChannelWidget::updateOkButton, mOkButton, &QPushButton::setEnabled);
}

CreateNewChannelDialog::~CreateNewChannelDialog()
{
    writeConfig();
}

CreateNewChannelDialog::NewChannelInfo CreateNewChannelDialog::channelInfo() const
{
    NewChannelInfo info;
    info.usersName = mCreateNewChannelWidget->users();
    info.channelName = mCreateNewChannelWidget->channelName();
    info.password = mCreateNewChannelWidget->password();
    info.readOnly = mCreateNewChannelWidget->readOnly();
    info.broadCast = mCreateNewChannelWidget->broadCast();
    info.privateChannel = mCreateNewChannelWidget->privateChannel();
    info.encryptedRoom = mCreateNewChannelWidget->encryptedRoom();
    return info;
}

void CreateNewChannelDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void CreateNewChannelDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}

void CreateNewChannelDialog::setFeatures(CreateNewChannelWidget::Features features)
{
    mCreateNewChannelWidget->setFeatures(features);
}
