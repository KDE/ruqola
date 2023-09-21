/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "roles/roleinfo.h"
#include <QStandardItemModel>
#include <QVector>
class LIBRUQOLACORE_EXPORT RolesModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum RolesInfoRoles {
        Name = Qt::UserRole + 1,
        Identifier,
    };
    Q_ENUM(RolesInfoRoles)

    explicit RolesModel(QObject *parent = nullptr);
    ~RolesModel() override;

    void setRoles(const QVector<RoleInfo> &newRoles);

    [[nodiscard]] QStringList rolesSelected() const;
    void setRolesSelected(const QStringList &newRolesSelected);

private:
    LIBRUQOLACORE_NO_EXPORT void createItem(const QString &displayStr, const QString &identifier);

    Q_DISABLE_COPY(RolesModel)
    QStringList mRolesSelected;
    void fillRoleSelected();
};
