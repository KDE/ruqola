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

#ifndef ROOMWRAPPER_H
#define ROOMWRAPPER_H

#include <QObject>
#include "room.h"
#include "libruqola_private_export.h"
class Room;
class LIBRUQOLACORE_EXPORT RoomWrapper : public QObject
{
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString topic READ topic NOTIFY topicChanged)
    Q_PROPERTY(QString announcement READ announcement NOTIFY announcementChanged)
    Q_PROPERTY(bool favorite READ favorite NOTIFY favoriteChanged)
    Q_OBJECT

public:
    explicit RoomWrapper(QObject *parent = nullptr);
    RoomWrapper(const Room &r, QObject *parent = nullptr);

    QString name() const;
    QString topic() const;
    QString announcement() const;    
    bool favorite() const;

Q_SIGNALS:
    void nameChanged();
    void topicChanged();
    void announcementChanged();
    void favoriteChanged();

private:
    Room mRoom;
};

#endif // ROOMWRAPPER_H
