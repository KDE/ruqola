/*
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright 2017-2018 Laurent Montel <montel@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "roomwrapper.h"
#include "ruqola_debug.h"
#include "room.h"
#include "notificationoptionswrapper.h"

RoomWrapper::RoomWrapper(QObject *parent)
    : QObject(parent)
{
}

RoomWrapper::RoomWrapper(Room *r, QObject *parent)
    : QObject(parent)
{
    mRoom = r;
    connect(mRoom, &Room::favoriteChanged, this, &RoomWrapper::favoriteChanged);
    connect(mRoom, &Room::topicChanged, this, &RoomWrapper::topicChanged);
    connect(mRoom, &Room::announcementChanged, this, &RoomWrapper::announcementChanged);
    connect(mRoom, &Room::nameChanged, this, &RoomWrapper::nameChanged);
    connect(mRoom, &Room::fnameChanged, this, &RoomWrapper::fnameChanged);
    connect(mRoom, &Room::readOnlyChanged, this, &RoomWrapper::readOnlyChanged);
    connect(mRoom, &Room::broadcastChanged, this, &RoomWrapper::broadcastChanged);
    connect(mRoom, &Room::blockerChanged, this, &RoomWrapper::blockerChanged);
    connect(mRoom, &Room::blockedChanged, this, &RoomWrapper::blockedChanged);
    connect(mRoom, &Room::encryptedChanged, this, &RoomWrapper::encryptedChanged);
    connect(mRoom, &Room::channelTypeChanged, this, &RoomWrapper::channelTypeChanged);

    //Reduce connection ?
    connect(mRoom, &Room::readOnlyChanged, this, &RoomWrapper::roomMessageInfoChanged);
    connect(mRoom, &Room::blockerChanged, this, &RoomWrapper::roomMessageInfoChanged);
    connect(mRoom, &Room::blockedChanged, this, &RoomWrapper::roomMessageInfoChanged);

    connect(mRoom, &Room::descriptionChanged, this, &RoomWrapper::descriptionChanged);
    connect(mRoom, &Room::archivedChanged, this, &RoomWrapper::archivedChanged);
    connect(mRoom, &Room::rolesChanged, this, &RoomWrapper::rolesChanged);
    connect(mRoom, &Room::ignoredUsersChanged, this, &RoomWrapper::ignoredUsersChanged);
    connect(mRoom, &Room::mutedUsersChanged, this, &RoomWrapper::mutedUsersChanged);
    connect(mRoom, &Room::joinCodeRequiredChanged, this, &RoomWrapper::joinCodeRequiredChanged);
}

RoomWrapper::~RoomWrapper()
{
}

QString RoomWrapper::name() const
{
    return mRoom->name();
}

QString RoomWrapper::fname() const
{
    return mRoom->fName();
}

QString RoomWrapper::displayRoomName() const
{
    return mRoom->displayRoomName();
}

QString RoomWrapper::topic() const
{
    return mRoom->topic();
}

QString RoomWrapper::announcement() const
{
    return mRoom->markdownAnnouncement();
}

bool RoomWrapper::favorite() const
{
    return mRoom->favorite();
}

bool RoomWrapper::readOnly() const
{
    return mRoom->readOnly();
}

bool RoomWrapper::blocker() const
{
    return mRoom->blocker();
}

bool RoomWrapper::blocked() const
{
    return mRoom->blocked();
}

bool RoomWrapper::encrypted() const
{
    return mRoom->encrypted();
}

bool RoomWrapper::broadcast() const
{
    return mRoom->broadcast();
}

QString RoomWrapper::channelType() const
{
    return mRoom->channelType();
}

QString RoomWrapper::roomCreatorUserId() const
{
    return mRoom->roomCreatorUserId();
}

QString RoomWrapper::rid() const
{
    return mRoom->roomId();
}

QString RoomWrapper::description() const
{
    return mRoom->description();
}

bool RoomWrapper::archived() const
{
    return mRoom->archived();
}

bool RoomWrapper::canBeModify() const
{
    return mRoom->canBeModify();
}

NotificationOptionsWrapper *RoomWrapper::notificationOptions() const
{
    return mRoom->getNotificationWrapper();
}

QString RoomWrapper::roomMessageInfo() const
{
    return mRoom->roomMessageInfo();
}

bool RoomWrapper::encryptedEnabled() const
{
    return mRoom->encryptedEnabled();
}

bool RoomWrapper::joinCodeRequired() const
{
    return mRoom->joinCodeRequired();
}

bool RoomWrapper::canChangeRoles() const
{
    return mRoom->canChangeRoles();
}

bool RoomWrapper::userHasOwnerRole(const QString &userId) const
{
    return mRoom->userHasOwnerRole(userId);
}

bool RoomWrapper::userHasLeaderRole(const QString &userId) const
{
    return mRoom->userHasLeaderRole(userId);
}

bool RoomWrapper::userHasModeratorRole(const QString &userId) const
{
    return mRoom->userHasModeratorRole(userId);
}
