/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef USERSFORROOMMODEL_H
#define USERSFORROOMMODEL_H

#include "libruqolacore_export.h"
#include "user.h"
#include <QVector>
#include <QAbstractListModel>
class UsersModel;
class LIBRUQOLACORE_EXPORT UsersForRoomModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum UsersForRoomRoles {
        UserName = Qt::UserRole + 1,
        UserId,
        Name,
        IconStatus,
        DisplayName
    };
    Q_ENUM(UsersForRoomRoles)

    explicit UsersForRoomModel(QObject *parent = nullptr);
    ~UsersForRoomModel() override;

    void setUsers(const QVector<User> &users);

    Q_INVOKABLE int rowCount(const QModelIndex &parent = {}) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    void parseUsersForRooms(const QJsonObject &root, UsersModel *model);
    void userStatusChanged(const User &newuser);
    void removeUser(const QString &userId);
    void addUser(const User &users);

    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;

private:
    QString generateDisplayName(const User &user) const;
    QVector<User> mUsers;
};

#endif // USERSFORROOMMODEL_H
