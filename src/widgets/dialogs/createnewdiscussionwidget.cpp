/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewdiscussionwidget.h"
#include "channelsearchwidget.h"
#include "misc/adduserswidget.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

CreateNewDiscussionWidget::CreateNewDiscussionWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mChannelSearchWidget(new ChannelSearchWidget(account, this))
    , mDiscussionNameLineEdit(new QLineEdit(this))
    , mUsers(new AddUsersWidget(account, this))
    , mMessageTextEdit(new KTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto channelLabel = new QLabel(i18n("Channel or Group parent"), this);
    channelLabel->setObjectName(QStringLiteral("channelLabel"));
    mainLayout->addWidget(channelLabel);

    mChannelSearchWidget->setObjectName(QStringLiteral("mChannelNameLineEdit"));
    connect(mChannelSearchWidget, &ChannelSearchWidget::updateRoomName, this, &CreateNewDiscussionWidget::updateOkButton);
    mainLayout->addWidget(mChannelSearchWidget);

    auto discussionName = new QLabel(i18n("Discussion Name"), this);
    discussionName->setObjectName(QStringLiteral("discussionName"));
    discussionName->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(discussionName);

    mDiscussionNameLineEdit->setObjectName(QStringLiteral("mDiscussionNameLineEdit"));
    KLineEditEventHandler::catchReturnKey(mDiscussionNameLineEdit);
    mDiscussionNameLineEdit->setClearButtonEnabled(true);
    mainLayout->addWidget(mDiscussionNameLineEdit);

    auto usersLabel = new QLabel(i18n("Invite Users"), this);
    usersLabel->setObjectName(QStringLiteral("usersLabel"));
    mainLayout->addWidget(usersLabel);

    mUsers->setObjectName(QStringLiteral("mUsers"));
    mUsers->setPlaceholderText(i18n("Invite users..."));
    mainLayout->addWidget(mUsers);

    auto messageLabel = new QLabel(i18n("Message"), this);
    messageLabel->setObjectName(QStringLiteral("messageLabel"));
    mainLayout->addWidget(messageLabel);

    mMessageTextEdit->setAcceptRichText(false);
    mMessageTextEdit->setObjectName(QStringLiteral("mMessageTextEdit"));
    mMessageTextEdit->setPlaceholderText(i18n("Usually a discussion starts with a question, like \"How do I upload a picture?\""));
    mainLayout->addWidget(mMessageTextEdit);
}

CreateNewDiscussionWidget::~CreateNewDiscussionWidget() = default;

void CreateNewDiscussionWidget::setChannelInfo(const QString &name, const QString &channelId)
{
    mChannelSearchWidget->setChannelInfo(name, channelId);
}

QString CreateNewDiscussionWidget::channelId() const
{
    return mChannelSearchWidget->channelId();
}

QString CreateNewDiscussionWidget::channelName() const
{
    return mChannelSearchWidget->channelName();
}

void CreateNewDiscussionWidget::setDiscussionName(const QString &name)
{
    mDiscussionNameLineEdit->setText(name);
}

QString CreateNewDiscussionWidget::discussionName() const
{
    return mDiscussionNameLineEdit->text();
}

void CreateNewDiscussionWidget::setMessage(const QString &name)
{
    mMessageTextEdit->setPlainText(name);
}

QString CreateNewDiscussionWidget::message() const
{
    return mMessageTextEdit->toPlainText();
}

QStringList CreateNewDiscussionWidget::usersId() const
{
    return mUsers->userIds();
}

#include "moc_createnewdiscussionwidget.cpp"
