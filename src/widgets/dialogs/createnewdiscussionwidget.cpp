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

#include "createnewdiscussionwidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <KLineEdit>
#include <KLocalizedString>
#include <QTextEdit>

CreateNewDiscussionWidget::CreateNewDiscussionWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *channelLabel = new QLabel(i18n("Channel or Group parent"), this);
    channelLabel->setObjectName(QStringLiteral("channelLabel"));
    mainLayout->addWidget(channelLabel);

    mChannelNameLineEdit = new KLineEdit(this);
    mChannelNameLineEdit->setObjectName(QStringLiteral("mChannelNameLineEdit"));
    mainLayout->addWidget(mChannelNameLineEdit);
    //TODO set readonly

    QLabel *discussionName = new QLabel(i18n("Discussion Name"), this);
    discussionName->setObjectName(QStringLiteral("discussionName"));
    mainLayout->addWidget(discussionName);

    mDiscussionNameLineEdit = new KLineEdit(this);
    mDiscussionNameLineEdit->setObjectName(QStringLiteral("mDiscussionNameLineEdit"));
    mainLayout->addWidget(mDiscussionNameLineEdit);

    QLabel *usersLabel = new QLabel(i18n("Invite Users"), this);
    discussionName->setObjectName(QStringLiteral("usersLabel"));
    mainLayout->addWidget(usersLabel);

    mUsersLineEdit = new KLineEdit(this);
    mUsersLineEdit->setObjectName(QStringLiteral("mUsersLineEdit"));
    mainLayout->addWidget(mUsersLineEdit);

    QLabel *messageLabel = new QLabel(i18n("Invite Users"), this);
    messageLabel->setObjectName(QStringLiteral("messageLabel"));
    mainLayout->addWidget(messageLabel);

    //TODO use ktextedit ? QPlainTextEdit ?
    mMessageTextEdit = new QTextEdit(this);
    mMessageTextEdit->setObjectName(QStringLiteral("mMessageTextEdit"));
    mainLayout->addWidget(mMessageTextEdit);

}

CreateNewDiscussionWidget::~CreateNewDiscussionWidget()
{

}
