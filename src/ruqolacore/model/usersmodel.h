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

#ifndef USERMODEL_H
#define USERMODEL_H

#include "libruqola_private_export.h"
#include <QAbstractListModel>
#include "user.h"

class LIBRUQOLACORE_TESTS_EXPORT UsersModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum UserRoles {
        UserName = Qt::UserRole + 1,
        UserId,
        UserStatus,
        UserIcon,
        UserStatusText
    };
    Q_ENUM(UserRoles)

    explicit UsersModel(QObject *parent = nullptr);
    ~UsersModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addUser(const User &userFromUserId);
    void removeUser(const QString &userId);

    void updateUser(const QJsonObject &array);
    Q_REQUIRED_RESULT QString userStatusIconFileName(const QString &name);

    Q_REQUIRED_RESULT QString status(const QString &userId) const;

Q_SIGNALS:
    void userStatusChanged(const User &user);
    void userNameChanged(const User &user);
    void nameChanged(const User &user);
    void statusMessageChanged(const User &user);

private:
    Q_DISABLE_COPY(UsersModel)
    QVector<User> mUsers;
};

#endif // USERMODEL_H
