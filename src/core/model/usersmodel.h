/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addUser(const User &userFromUserId);
    void removeUser(const QString &userId);

    void updateUser(const QJsonObject &array);
    [[nodiscard]] QString userStatusIconFileName(const QString &name) const;

    [[nodiscard]] User::PresenceStatus status(const QString &userId) const;
    [[nodiscard]] User fullUserInfo(const QString &userName) const;
    [[nodiscard]] bool userIsOffline(const QString &name) const;

    [[nodiscard]] QString userStatusStr(const QString &name) const;
Q_SIGNALS:
    void userStatusChanged(const User &user);
    void userNameChanged(const User &user);
    void nameChanged(const User &user);
    void statusMessageChanged(const User &user);

private:
    Q_DISABLE_COPY(UsersModel)
    QVector<User> mUsers;
};
