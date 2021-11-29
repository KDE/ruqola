/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "user.h"
#include <QAbstractListModel>
class LIBRUQOLACORE_TESTS_EXPORT UsersModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum UserRoles {
        UserName = Qt::UserRole + 1,
        UserId,
        UserStatus,
        UserIcon,
        UserStatusText,
    };
    Q_ENUM(UserRoles)

    explicit UsersModel(QObject *parent = nullptr);
    ~UsersModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addUser(const User &userFromUserId);
    void removeUser(const QString &userId);

    void updateUser(const QJsonObject &array);
    Q_REQUIRED_RESULT QString userStatusIconFileName(const QString &name) const;

    Q_REQUIRED_RESULT User::PresenceStatus status(const QString &userId) const;
    Q_REQUIRED_RESULT User fullUserInfo(const QString &userName) const;
    Q_REQUIRED_RESULT bool userIsOffline(const QString &name) const;

    Q_REQUIRED_RESULT QString userStatusStr(const QString &name) const;
Q_SIGNALS:
    void userStatusChanged(const User &user);
    void userNameChanged(const User &user);
    void nameChanged(const User &user);
    void statusMessageChanged(const User &user);

private:
    Q_DISABLE_COPY(UsersModel)
    QVector<User> mUsers;
};

