/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
CreateNewChannelDialog::CreateNewChannelDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mCreateNewChannelWidget(new CreateNewChannelWidget(account, this))
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
