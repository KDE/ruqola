/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "directorybasemodel.h"
#include "libruqolacore_export.h"
#include "roles/roleinfo.h"
#include "users.h"

class LIBRUQOLACORE_EXPORT DirectoryUsersModel : public DirectoryBaseModel
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

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;
    Q_REQUIRED_RESULT QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_REQUIRED_RESULT int total() const override;

    void parseElements(const QJsonObject &obj) override;
    void addMoreElements(const QJsonObject &obj) override;

    Q_REQUIRED_RESULT int columnCount(const QModelIndex &parent = {}) const override;

    void setRoles(const QVector<RoleInfo> &newRoles);

protected:
    Q_REQUIRED_RESULT QList<int> hideColumns() const override;
    Q_REQUIRED_RESULT virtual Users::ParseType parseType() const;
    Users mUsers;
    QVector<RoleInfo> mRoleInfo;
    void checkFullList();

private:
    Q_DISABLE_COPY(DirectoryUsersModel)
};
