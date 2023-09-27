/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "custombasemodel.h"
#include "libruqolacore_export.h"
#include "roles/roleinfo.h"
#include "users.h"

class LIBRUQOLACORE_EXPORT DirectoryUsersModel : public CustomBaseModel
{
    Q_OBJECT
public:
    enum DirectoryUsersRoles {
        Name,
        Email,
        JoinAt,
        UserId,
        JoinAtDateTime,
        UserName,
        LastColumn = UserName,
    };
    Q_ENUM(DirectoryUsersRoles)
    explicit DirectoryUsersModel(QObject *parent = nullptr);
    ~DirectoryUsersModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] int total() const override;

    void parseElements(const QJsonObject &obj) override;
    void addMoreElements(const QJsonObject &obj) override;

    [[nodiscard]] int columnCount(const QModelIndex &parent = {}) const override;

    void setRoles(const QVector<RoleInfo> &newRoles);

protected:
    [[nodiscard]] QList<int> hideColumns() const override;
    [[nodiscard]] virtual Users::ParseType parseType() const;
    Users mUsers;
    QVector<RoleInfo> mRoleInfo;
    void checkFullList() override;

private:
    Q_DISABLE_COPY(DirectoryUsersModel)
};
