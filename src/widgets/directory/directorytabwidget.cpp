/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "directorytabwidget.h"
#include "directorywidget.h"
#include <KLocalizedString>

DirectoryTabWidget::DirectoryTabWidget(RocketChatAccount *account, QWidget *parent)
    : QTabWidget(parent)
    , mRooms(new DirectoryWidget(account, DirectoryWidget::Room, this))
    , mUsers(new DirectoryWidget(account, DirectoryWidget::User, this))
    , mTeams(new DirectoryWidget(account, DirectoryWidget::Team, this))
{
    mRooms->setObjectName(QStringLiteral("mRooms"));
    mUsers->setObjectName(QStringLiteral("mUsers"));
    mTeams->setObjectName(QStringLiteral("mTeams"));
    addTab(mRooms, i18n("Rooms"));
    addTab(mUsers, i18n("Users"));
    addTab(mTeams, i18n("Teams"));
}

DirectoryTabWidget::~DirectoryTabWidget() = default;

void DirectoryTabWidget::fillTabs()
{
    mRooms->fillDirectory();
    mUsers->fillDirectory();
    mTeams->fillDirectory();
}
