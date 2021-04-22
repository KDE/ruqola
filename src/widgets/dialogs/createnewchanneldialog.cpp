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

#include "createnewchanneldialog.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{
static const char myCreateNewChannelDialogGroupName[] = "CreateNewChannelDialog";
}
CreateNewChannelDialog::CreateNewChannelDialog(QWidget *parent)
    : QDialog(parent)
    , mCreateNewChannelWidget(new CreateNewChannelWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Create Channel"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateNewChannelWidget->setObjectName(QStringLiteral("mCreateNewChannelWidget"));
    mainLayout->addWidget(mCreateNewChannelWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
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
    CreateNewChannelDialog::NewChannelInfo newChannelInfo;
    RocketChatRestApi::CreateRoomInfo info;
    info.members = mCreateNewChannelWidget->users();
    info.name = mCreateNewChannelWidget->channelName();
    info.password = mCreateNewChannelWidget->password();
    info.readOnly = mCreateNewChannelWidget->readOnly();
    info.broadcast = mCreateNewChannelWidget->broadCast();
    info.encrypted = mCreateNewChannelWidget->encryptedRoom();
    info.description = mCreateNewChannelWidget->topic();

    newChannelInfo.info = info;
    newChannelInfo.privateChannel = mCreateNewChannelWidget->privateChannel();
    return newChannelInfo;
}

void CreateNewChannelDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myCreateNewChannelDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void CreateNewChannelDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myCreateNewChannelDialogGroupName);
    group.writeEntry("Size", size());
}

void CreateNewChannelDialog::setFeatures(CreateNewChannelWidget::Features features)
{
    mCreateNewChannelWidget->setFeatures(features);
}
