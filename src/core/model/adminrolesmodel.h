/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "roles/roleinfo.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT AdminRolesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AdminRoles {
        Identifier,
        Name,
        Scope,
        Protected,
        Mandatory2Fa,
        Description,
        LastColumn = Description,
    };
    Q_ENUM(AdminRoles)

    explicit AdminRolesModel(QObject *parent = nullptr);
    ~AdminRolesModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    void setRoles(const QList<RoleInfo> &newRoles);

    [[nodiscard]] QList<RoleInfo> roles() const;

private:
    LIBRUQOLACORE_NO_EXPORT void clear();
    QList<RoleInfo> mListRoleInfos;
};
