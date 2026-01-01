/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "adminusersbasemodel.h"

#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT AdminUsersStatusModel : public AdminUsersBaseModel
{
    Q_OBJECT
public:
    enum AdminUsersAllRoles {
        Name,
        UserName,
        Email,
        Roles,
        UserId,
        Administrator,
        LastColumn = Administrator,
    };
    Q_ENUM(AdminUsersAllRoles)

    explicit AdminUsersStatusModel(QObject *parent = nullptr);
    ~AdminUsersStatusModel() override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QList<int> hideColumns() const override;
};
