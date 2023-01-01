/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include "user.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_TESTS_EXPORT UserCompleterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum UserRoles {
        UserName = Qt::UserRole + 1,
        UserId,
        UserIconStatus,
    };
    Q_ENUM(UserRoles)

    explicit UserCompleterModel(QObject *parent = nullptr);
    ~UserCompleterModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();
    void insertUsers(const QVector<User> &users);

private:
    Q_REQUIRED_RESULT QString displayUserName(const User &user) const;
    Q_DISABLE_COPY(UserCompleterModel)
    QVector<User> mUsers;
};
