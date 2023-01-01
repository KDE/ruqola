/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewchanneldialog.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

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

RocketChatRestApi::CreateChannelTeamInfo CreateNewChannelDialog::channelInfo(bool userMemberUserId) const
{
    RocketChatRestApi::CreateChannelTeamInfo info;
    info.members = mCreateNewChannelWidget->members(userMemberUserId);
    info.name = mCreateNewChannelWidget->channelName();
    info.readOnly = mCreateNewChannelWidget->readOnly();
    info.broadcast = mCreateNewChannelWidget->broadCast();
    info.encrypted = mCreateNewChannelWidget->encryptedRoom();
    info.description = mCreateNewChannelWidget->topic();
    info.privateChannel = mCreateNewChannelWidget->privateChannel();
    return info;
}

void CreateNewChannelDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), myCreateNewChannelDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void CreateNewChannelDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myCreateNewChannelDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void CreateNewChannelDialog::setFeatures(CreateNewChannelWidget::Features features)
{
    mCreateNewChannelWidget->setFeatures(features);
}
