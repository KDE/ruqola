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

#include "createnewdiscussiondialog.h"
#include "createnewdiscussionwidget.h"
#include "rocketchataccount.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{
static const char myCreateNewDiscussionDialogGroupName[] = "CreateNewDiscussionDialog";
}
CreateNewDiscussionDialog::CreateNewDiscussionDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mCreateNewDiscussionWidget(new CreateNewDiscussionWidget(this))
    , mCurrentRocketChatAccount(account)
{
    setWindowTitle(i18nc("@title:window", "Create Discussion"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateNewDiscussionWidget->setObjectName(QStringLiteral("mCreateNewDiscussionWidget"));
    mainLayout->addWidget(mCreateNewDiscussionWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &CreateNewDiscussionDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &CreateNewDiscussionDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
    QPushButton *button = buttonBox->button(QDialogButtonBox::Ok);
    button->setText(i18n("Create"));
    button->setEnabled(false);
    connect(mCreateNewDiscussionWidget, &CreateNewDiscussionWidget::updateOkButton, this, [button](bool state) {
        button->setEnabled(state);
    });
    connect(buttonBox, &QDialogButtonBox::accepted, this, &CreateNewDiscussionDialog::createNewDiscussion);
}

CreateNewDiscussionDialog::~CreateNewDiscussionDialog()
{
    writeConfig();
}

void CreateNewDiscussionDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myCreateNewDiscussionDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void CreateNewDiscussionDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myCreateNewDiscussionDialogGroupName);
    group.writeEntry("Size", size());
}

CreateNewDiscussionDialog::NewDiscussionInfo CreateNewDiscussionDialog::newDiscussionInfo() const
{
    NewDiscussionInfo info;
    info.channelId = mCreateNewDiscussionWidget->channelId();
    info.channelName = mCreateNewDiscussionWidget->channelName();
    info.message = mCreateNewDiscussionWidget->message();
    info.users = mCreateNewDiscussionWidget->usersId();
    info.discussionName = mCreateNewDiscussionWidget->discussionName();
    return info;
}

void CreateNewDiscussionDialog::setChannelInfo(const QString &name, const QString &channelId)
{
    mCreateNewDiscussionWidget->setChannelInfo(name, channelId);
}

void CreateNewDiscussionDialog::setDiscussionName(const QString &name)
{
    mCreateNewDiscussionWidget->setDiscussionName(name);
}

void CreateNewDiscussionDialog::createNewDiscussion()
{
    const CreateNewDiscussionDialog::NewDiscussionInfo info = newDiscussionInfo();
    mCurrentRocketChatAccount->createDiscussion(info.channelId, info.discussionName, info.message, mMessageId, info.users);
    accept();
}

const QString &CreateNewDiscussionDialog::messageId() const
{
    return mMessageId;
}

void CreateNewDiscussionDialog::setMessageId(const QString &newMessageId)
{
    mMessageId = newMessageId;
}
