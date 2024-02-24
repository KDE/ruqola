/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "user.h"
#include "utils.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_TESTS_EXPORT UserCompleterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum UserRoles {
        UserName = Qt::UserRole + 1,
        DisplayName,
        UserId,
        UserIconStatus,
        AvatarInfo,
    };
    Q_ENUM(UserRoles)

    explicit UserCompleterModel(QObject *parent = nullptr);
    ~UserCompleterModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();
    void addUsers(const QList<User> &users);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString displayUserName(const User &user) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT Utils::AvatarInfo avatarInfo(const User &user) const;
    Q_DISABLE_COPY(UserCompleterModel)
    QList<User> mUsers;
};
