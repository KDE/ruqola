/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "adminusersbasemodel.h"

#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT AdminUsersAllModel : public AdminUsersBaseModel
{
    Q_OBJECT
public:
    enum AdminUsersAllRoles {
        Name,
        UserName,
        Email,
        Roles,
        Status,
        UserId,
        ActiveUserDisplay,
        ActiveUser,
        Administrator,
        RegistrationStatus,
        LastColumn = RegistrationStatus,
    };
    Q_ENUM(AdminUsersAllRoles)

    explicit AdminUsersAllModel(QObject *parent = nullptr);
    ~AdminUsersAllModel() override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QList<int> hideColumns() const override;
    [[nodiscard]] bool setData(const QModelIndex &index, const QVariant &value, int role) override;

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString createRegistrationStatus(const User &user) const;
};
