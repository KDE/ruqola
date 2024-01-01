/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "custombasemodel.h"
#include "libruqolacore_export.h"
#include "users.h"

class LIBRUQOLACORE_EXPORT UsersInRoleModel : public CustomBaseModel
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

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] int total() const override;

    void parseElements(const QJsonObject &obj) override;
    void addMoreElements(const QJsonObject &obj) override;

    int columnCount(const QModelIndex &parent = {}) const override;

protected:
    [[nodiscard]] QList<int> hideColumns() const override;

private:
    Q_DISABLE_COPY(UsersInRoleModel)
    LIBRUQOLACORE_NO_EXPORT void checkFullList() override;
    Users mUsers;
};
