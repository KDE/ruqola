/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewchanneldialog.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);

    mCreateNewChannelWidget->setObjectName(u"mCreateNewChannelWidget"_s);
    mainLayout->addWidget(mCreateNewChannelWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::accepted, this, &CreateNewChannelDialog::accept);
    connect(button, &QDialogButtonBox::rejected, this, &CreateNewChannelDialog::reject);
    readConfig();
    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    okButton->setText(i18nc("@action:button", "Create"));
    connect(mCreateNewChannelWidget, &CreateNewChannelWidget::updateOkButton, okButton, &QPushButton::setEnabled);
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
    info.topic = mCreateNewChannelWidget->topic();
    info.privateChannel = mCreateNewChannelWidget->privateChannel();
    info.federated = mCreateNewChannelWidget->federated();
    return info;
}

void CreateNewChannelDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myCreateNewChannelDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void CreateNewChannelDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myCreateNewChannelDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void CreateNewChannelDialog::setFeatures(CreateNewChannelWidget::Features features)
{
    mCreateNewChannelWidget->setFeatures(features);
}

#include "moc_createnewchanneldialog.cpp"
