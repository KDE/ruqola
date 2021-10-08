/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#include "otrmanager.h"
#include "otrnotificationjob.h"
#include "ruqola_debug.h"
#include <KLocalizedString>

OtrManager::OtrManager(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mRocketChatAccount(account)
{
}

OtrManager::~OtrManager()
{
}

void OtrManager::slotActivateNotificationAction()
{
}

void OtrManager::parseOtr(const QJsonArray &contents)
{
    Otr t;
    t.parseOtr(contents);
    auto job = new OtrNotificationJob(this);
    job->setRocketChatAccount(mRocketChatAccount);
    job->setOtr(t);
    job->start();
}
