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

#include "createnewdiscussionwidget.h"
#include "channelsearchwidget.h"
#include "misc/adduserswidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include <KLocalizedString>
#include <KTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

CreateNewDiscussionWidget::CreateNewDiscussionWidget(QWidget *parent)
    : QWidget(parent)
    , mChannelSearchWidget(new ChannelSearchWidget(this))
    , mDiscussionNameLineEdit(new QLineEdit(this))
    , mUsers(new AddUsersWidget(this))
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
    new LineEditCatchReturnKey(mDiscussionNameLineEdit, this);
    mDiscussionNameLineEdit->setClearButtonEnabled(true);
    mainLayout->addWidget(mDiscussionNameLineEdit);

    auto usersLabel = new QLabel(i18n("Invite Users"), this);
    usersLabel->setObjectName(QStringLiteral("usersLabel"));
    mainLayout->addWidget(usersLabel);

    mUsers->setObjectName(QStringLiteral("mUsers"));
    mUsers->setPlaceholderText(i18n("Invite Users..."));
    mainLayout->addWidget(mUsers);

    auto messageLabel = new QLabel(i18n("Message"), this);
    messageLabel->setObjectName(QStringLiteral("messageLabel"));
    mainLayout->addWidget(messageLabel);

    mMessageTextEdit->setAcceptRichText(false);
    mMessageTextEdit->setObjectName(QStringLiteral("mMessageTextEdit"));
    mainLayout->addWidget(mMessageTextEdit);
}

CreateNewDiscussionWidget::~CreateNewDiscussionWidget()
{
}

void CreateNewDiscussionWidget::setChannelName(const QString &name)
{
    mChannelSearchWidget->setChannelName(name);
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
