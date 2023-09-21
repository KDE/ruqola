/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "directoryusersmodel.h"

#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT AdminUsersModel : public DirectoryUsersModel
{
    Q_OBJECT
public:
    enum AdminUsersRoles {
        Name,
        UserName,
        Email,
        Roles,
        Status,
        UserId,
        ActiveUserDisplay,
        ActiveUser,
        Administrator,
        LastColumn = Administrator,
    };
    Q_ENUM(AdminUsersRoles)

    explicit AdminUsersModel(QObject *parent = nullptr);
    ~AdminUsersModel() override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QList<int> hideColumns() const override;
    [[nodiscard]] bool setData(const QModelIndex &index, const QVariant &value, int role) override;

protected:
    [[nodiscard]] Users::ParseType parseType() const override;
    void removeElement(const QString &identifier) override;
    void updateElement(const QJsonObject &obj) override;
    void insertElement(const QJsonObject &obj) override;
};
