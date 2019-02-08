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
    Q_PROPERTY(QString channelType READ channelType NOTIFY channelTypeChanged)
    Q_PROPERTY(QString roomCreatorUserId READ roomCreatorUserId CONSTANT)
    Q_PROPERTY(QString rid READ rid CONSTANT)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(bool favorite READ favorite NOTIFY favoriteChanged)
    Q_PROPERTY(bool readOnly READ readOnly NOTIFY readOnlyChanged)
    Q_PROPERTY(bool blocker READ blocker NOTIFY blockerChanged)
    Q_PROPERTY(bool blocked READ blocked NOTIFY blockedChanged)
    Q_PROPERTY(bool encrypted READ encrypted NOTIFY encryptedChanged)
    Q_PROPERTY(bool canBeModify READ canBeModify NOTIFY rolesChanged)
    Q_PROPERTY(bool archived READ archived NOTIFY archivedChanged)
    //TODO add Q_PROPERTY for mutedUsersChanged and ignoreUsersChanged
    Q_PROPERTY(QString roomMessageInfo READ roomMessageInfo NOTIFY roomMessageInfoChanged)
    Q_PROPERTY(bool encryptedEnabled READ encryptedEnabled CONSTANT)
    Q_PROPERTY(bool joinCodeRequired READ joinCodeRequired NOTIFY joinCodeRequiredChanged)
    Q_PROPERTY(bool canChangeRoles READ canChangeRoles CONSTANT)
    Q_OBJECT
public:
    explicit RoomWrapper(QObject *parent = nullptr);
    explicit RoomWrapper(Room *r, QObject *parent = nullptr);
    ~RoomWrapper();

    Q_REQUIRED_RESULT QString name() const;
    Q_REQUIRED_RESULT QString topic() const;
    Q_REQUIRED_RESULT QString announcement() const;
    Q_REQUIRED_RESULT bool favorite() const;
    Q_REQUIRED_RESULT bool readOnly() const;
    Q_REQUIRED_RESULT bool blocker() const;
    Q_REQUIRED_RESULT bool blocked() const;
    Q_REQUIRED_RESULT bool encrypted() const;
    Q_REQUIRED_RESULT QString channelType() const;
    Q_REQUIRED_RESULT QString roomCreatorUserId() const;
    Q_REQUIRED_RESULT QString rid() const;
    Q_REQUIRED_RESULT QString description() const;
    Q_REQUIRED_RESULT bool archived() const;
    Q_REQUIRED_RESULT bool canBeModify() const;
    Q_REQUIRED_RESULT QString roomMessageInfo() const;

    Q_REQUIRED_RESULT bool encryptedEnabled() const;
    Q_REQUIRED_RESULT bool joinCodeRequired() const;

    Q_REQUIRED_RESULT Q_INVOKABLE NotificationOptionsWrapper *notificationOptions() const;
    Q_REQUIRED_RESULT bool canChangeRoles() const;
Q_SIGNALS:
    void nameChanged();
    void topicChanged();
    void announcementChanged();
    void favoriteChanged();
    void readOnlyChanged();
    void blockerChanged();
    void descriptionChanged();
    void archivedChanged();
    void blockedChanged();
    void rolesChanged();
    void ignoredUsersChanged();
    void mutedUsersChanged();
    void roomMessageInfoChanged();
    void encryptedChanged();
    void joinCodeRequiredChanged();
    void channelTypeChanged();

private:
    Q_DISABLE_COPY(RoomWrapper)
    Room *mRoom = nullptr;
};

#endif // ROOMWRAPPER_H
