/*
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright 2017  Laurent Montel <montel@kde.org>
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
#include "room.h"

RoomWrapper::RoomWrapper(QObject *parent)
    : QObject(parent)
{
}

RoomWrapper::RoomWrapper(const Room &r, QObject *parent)
    : QObject(parent)
{
    mRoom = r;
}

QString RoomWrapper::name() const
{
    return mRoom.name();
}

QString RoomWrapper::topic() const
{
    return mRoom.topic();
}

QString RoomWrapper::announcement() const
{
    return mRoom.announcement();
}

bool RoomWrapper::favorite() const
{
    return mRoom.favorite();
}
