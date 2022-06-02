/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    , mCreateNewDiscussionWidget(new CreateNewDiscussionWidget(account, this))
    , mCurrentRocketChatAccount(account)
{
    setWindowTitle(i18nc("@title:window", "Create Discussion - %1", account ? account->accountName() : QStringLiteral("account")));
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
