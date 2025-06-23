/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewdiscussionwidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto channelLabel = new QLabel(i18nc("@label:textbox", "Channel or Group parent"), this);
    channelLabel->setObjectName(u"channelLabel"_s);
    mainLayout->addWidget(channelLabel);

    mChannelSearchWidget->setObjectName(u"mChannelNameLineEdit"_s);
    connect(mChannelSearchWidget, &ChannelSearchWidget::updateRoomName, this, &CreateNewDiscussionWidget::updateOkButton);
    mainLayout->addWidget(mChannelSearchWidget);

    auto discussionName = new QLabel(i18nc("@label:textbox", "Discussion Name"), this);
    discussionName->setObjectName(u"discussionName"_s);
    discussionName->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(discussionName);

    mDiscussionNameLineEdit->setObjectName(u"mDiscussionNameLineEdit"_s);
    KLineEditEventHandler::catchReturnKey(mDiscussionNameLineEdit);
    mDiscussionNameLineEdit->setClearButtonEnabled(true);
    mainLayout->addWidget(mDiscussionNameLineEdit);

    auto usersLabel = new QLabel(i18nc("@label:textbox", "Invite Users"), this);
    usersLabel->setObjectName(u"usersLabel"_s);
    mainLayout->addWidget(usersLabel);

    mUsers->setObjectName(u"mUsers"_s);
    mUsers->setPlaceholderText(i18nc("@info:placeholder", "Invite users…"));
    mainLayout->addWidget(mUsers);

    auto messageLabel = new QLabel(i18nc("@label:textbox", "Message"), this);
    messageLabel->setObjectName(u"messageLabel"_s);
    mainLayout->addWidget(messageLabel);

    mMessageTextEdit->setAcceptRichText(false);
    mMessageTextEdit->setTabChangesFocus(true);
    mMessageTextEdit->setObjectName(u"mMessageTextEdit"_s);
    mMessageTextEdit->setPlaceholderText(i18nc("@info:placeholder", "Usually a discussion starts with a question, like \"How do I upload a picture?\""));
    mainLayout->addWidget(mMessageTextEdit);
}

CreateNewDiscussionWidget::~CreateNewDiscussionWidget() = default;

void CreateNewDiscussionWidget::setChannelInfo(const QString &name, const QByteArray &channelId)
{
    mChannelSearchWidget->setChannelInfo(name, channelId);
}

QByteArray CreateNewDiscussionWidget::channelId() const
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

QList<QByteArray> CreateNewDiscussionWidget::usersId() const
{
    return mUsers->userIds();
}

#include "moc_createnewdiscussionwidget.cpp"
