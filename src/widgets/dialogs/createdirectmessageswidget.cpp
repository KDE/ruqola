/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createdirectmessageswidget.h"
#include "misc/adduserswidget.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

CreateDirectMessagesWidget::CreateDirectMessagesWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mUsers(new AddUsersWidget(account, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18n("You are about to create a chat with multiple users. Add the ones you would like to talk, "
                                 "everyone in the same place, using direct messages."),
                            this);
    label->setObjectName(QStringLiteral("label"));
    label->setWordWrap(true);
    mainLayout->addWidget(label);

    mUsers->setObjectName(QStringLiteral("mUsers"));
    mUsers->setPlaceholderText(i18n("Select users..."));
    mainLayout->addWidget(mUsers);
    mainLayout->addStretch(1);
    connect(mUsers, &AddUsersWidget::userListChanged, this, &CreateDirectMessagesWidget::updateOkButton);
}

CreateDirectMessagesWidget::~CreateDirectMessagesWidget() = default;

QStringList CreateDirectMessagesWidget::userNames() const
{
    return mUsers->userNames();
}
