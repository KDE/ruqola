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

#include "room.h"
#include <QDebug>
Room::Room()
{

}

bool Room::isEqual(const Room &other) const
{
    qDebug() << " id " << id  << " other " << other.id;
    qDebug() << " type " << type  << " type " << other.type;
    qDebug() << " name " << name  << " name " << other.name;
    qDebug() << " mAnnouncement " << mAnnouncement  << " mAnnouncement " << other.mAnnouncement;
    qDebug() << " userName " << userName  << " userName " << other.userName;
    qDebug() << " userID " << userID  << " userID " << other.userID;
    qDebug() << " topic " << topic  << " topic " << other.topic;
    qDebug() << " mutedUsers " << mutedUsers  << " mutedUsers " << other.mutedUsers;
    qDebug() << " jitsiTimeout " << jitsiTimeout  << " jitsiTimeout " << other.jitsiTimeout;
    qDebug() << " ro " << ro  << " ro " << other.ro;
    qDebug() << " unread " << unread  << " unread " << other.unread;
    qDebug() << " selected " << selected  << " selected " << other.selected;
    return (id == other.id) &&
            (type == other.type) &&
            (name == other.name) &&
            (mAnnouncement == other.mAnnouncement) &&
            (userName == other.userName) &&
            (userID == other.userID) &&
            (topic == other.topic) &&
            (mutedUsers == other.mutedUsers) &&
            (jitsiTimeout == other.jitsiTimeout) &&
            (ro == other.ro) &&
            (unread == other.unread) &&
            (selected == other.selected);
}
