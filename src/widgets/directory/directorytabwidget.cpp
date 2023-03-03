/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "directorytabwidget.h"
#include "directorywidget.h"
#include "rocketchataccount.h"
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
    const int roomsTabIndex = addTab(mRooms, i18n("Rooms"));
    const int usersTabIndex = addTab(mUsers, i18n("Users"));
    const int teamsTabIndex = addTab(mTeams, i18n("Teams"));

    if (account && !account->hasPermission(QStringLiteral("view-c-room"))) {
        setTabVisible(roomsTabIndex, false);
        setTabVisible(teamsTabIndex, false);
    }
    if (account && (!account->hasPermission(QStringLiteral("view-outside-room")) || !account->hasPermission(QStringLiteral("view-d-room")))) {
        setTabVisible(usersTabIndex, false);
    }
    // TODO show default page for informing that we don't have access.
}

DirectoryTabWidget::~DirectoryTabWidget() = default;

void DirectoryTabWidget::fillTabs()
{
    if (isTabVisible(indexOf(mRooms))) {
        mRooms->fillDirectory();
        mTeams->fillDirectory();
    }
    if (isTabVisible(indexOf(mUsers))) {
        mUsers->fillDirectory();
    }
}
