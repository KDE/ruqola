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

#include "createdirectmessageswidget.h"
#include "misc/adduserswidget.h"
#include <KLocalizedString>
#include <QVBoxLayout>
#include <QLabel>

CreateDirectMessagesWidget::CreateDirectMessagesWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    QLabel *label = new QLabel(i18n("You are about to create a chat with multiple users. Add the ones you would like to talk, "
                                    "everyone in the same place, using direct messages."), this);
    label->setObjectName(QStringLiteral("label"));
    label->setWordWrap(true);
    mainLayout->addWidget(label);

    mUsers = new AddUsersWidget(this);
    mUsers->setObjectName(QStringLiteral("mUsers"));
    mUsers->setPlaceholderText(i18n("Select Users..."));
    mainLayout->addWidget(mUsers);
    mainLayout->addStretch(1);
    connect(mUsers, &AddUsersWidget::userListChanged, this, &CreateDirectMessagesWidget::updateOkButton);
}

CreateDirectMessagesWidget::~CreateDirectMessagesWidget()
{
}

QStringList CreateDirectMessagesWidget::userNames() const
{
    return mUsers->userNames();
}
