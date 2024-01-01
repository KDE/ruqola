/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "directorytabwidget.h"
#include "directorystackedwidget.h"
#include "rocketchataccount.h"
#include <KLocalizedString>

DirectoryTabWidget::DirectoryTabWidget(RocketChatAccount *account, QWidget *parent)
    : QTabWidget(parent)
    , mRooms(new DirectoryStackedWidget(account, DirectoryWidget::Room, this))
    , mUsers(new DirectoryStackedWidget(account, DirectoryWidget::User, this))
    , mTeams(new DirectoryStackedWidget(account, DirectoryWidget::Team, this))
{
    mRooms->setObjectName(QStringLiteral("mRooms"));
    mUsers->setObjectName(QStringLiteral("mUsers"));
    mTeams->setObjectName(QStringLiteral("mTeams"));
    addTab(mRooms, i18n("Rooms"));
    addTab(mUsers, i18n("Users"));
    addTab(mTeams, i18n("Teams"));

    if (account && !account->hasPermission(QStringLiteral("view-c-room"))) {
        mRooms->setIsAutorized(false);
        mTeams->setIsAutorized(false);
    }
    if (account && (!account->hasPermission(QStringLiteral("view-outside-room")) || !account->hasPermission(QStringLiteral("view-d-room")))) {
        mUsers->setIsAutorized(false);
    }
}

DirectoryTabWidget::~DirectoryTabWidget() = default;

void DirectoryTabWidget::fillTabs()
{
    mRooms->fillDirectory();
    mTeams->fillDirectory();
    mUsers->fillDirectory();
}

#include "moc_directorytabwidget.cpp"
