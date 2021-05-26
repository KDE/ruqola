/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "directorytabwidget.h"
#include "directorywidget.h"
#include <KLocalizedString>

DirectoryTabWidget::DirectoryTabWidget(QWidget *parent)
    : QTabWidget(parent)
    , mRooms(new DirectoryWidget(DirectoryWidget::Room, this))
    , mUsers(new DirectoryWidget(DirectoryWidget::User, this))
    , mTeams(new DirectoryWidget(DirectoryWidget::Team, this))
{
    mRooms->setObjectName(QStringLiteral("mRooms"));
    mUsers->setObjectName(QStringLiteral("mUsers"));
    mTeams->setObjectName(QStringLiteral("mTeams"));
    addTab(mRooms, i18n("Rooms"));
    addTab(mUsers, i18n("Users"));
    addTab(mTeams, i18n("Teams"));
}

DirectoryTabWidget::~DirectoryTabWidget()
{
}

void DirectoryTabWidget::fillTabs()
{
    mRooms->fillDirectory();
    mUsers->fillDirectory();
    mTeams->fillDirectory();
}
