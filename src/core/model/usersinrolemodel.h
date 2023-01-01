/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "directorybasemodel.h"
#include "libruqolacore_export.h"
#include "users.h"

class LIBRUQOLACORE_EXPORT UsersInRoleModel : public DirectoryBaseModel
{
    Q_OBJECT
public:
    enum UsersInRoleRoles {
        Name,
        Email,
        UserId,
        UserName,
        LastColumn = UserName,
    };
    Q_ENUM(UsersInRoleRoles)
    explicit UsersInRoleModel(QObject *parent = nullptr);
    ~UsersInRoleModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;
    Q_REQUIRED_RESULT QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_REQUIRED_RESULT int total() const override;

    void parseElements(const QJsonObject &obj) override;
    void addMoreElements(const QJsonObject &obj) override;

    int columnCount(const QModelIndex &parent = {}) const override;

protected:
    Q_REQUIRED_RESULT QList<int> hideColumns() const override;

private:
    Q_DISABLE_COPY(UsersInRoleModel)
    void checkFullList();
    Users mUsers;
};
