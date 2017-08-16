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

#ifndef ROOM_H
#define ROOM_H

#include <QJsonObject>
#include <QString>

#include "libruqola_private_export.h"

class LIBRUQOLACORE_TESTS_EXPORT Room
{
public:
    Room();

    // To be used in ID find: message ID
    bool operator==(const Room &other) const;

    // To be used in sorted insert: timestamp
    bool operator<(const Room &other) const;

    //we can't use operator== as it tests only id. We need it for autotest
    bool isEqual(const Room &other) const;

    /**
    * @brief Return room name
    *
    * @return QString, The name of the room
    */
    QString getName() const
    {
        return name;
    }

    /**
    * @brief Return topic name
    *
    * @return QString, The name of the topic of room
    */
    QString getTopic() const
    {
        return topic;
    }

    void parseSubscriptionRoom(const QJsonObject &json);
    void parseRoom(const QJsonObject &json);


    //Room Object Fields

    // _id
    QString id;

    // t (can take values "d" , "c" or "p")
    QString type;

    // name
    QString name;

    // Announcement
    QString mAnnouncement;

    // u
    QString userName;
    QString userID;

    // topic
    QString topic;

    // muted - collection of muted users by its usernames
    QString mutedUsers; // --> TODO: this really is a list - it requires some more work

    // jitsiTimeout
    qint64 jitsiTimeout = -1;

    // ro - read-only chat or not
    bool ro = false;

    int unread = -1;
    bool selected = false;
    bool favorite = false;
    //We can hide it or not.
    bool open = false;

    bool alert = false;
};

LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Room &t);
#endif // ROOM_H
