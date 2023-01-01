/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;
    Q_REQUIRED_RESULT QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_REQUIRED_RESULT int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    void setRoles(const QVector<RoleInfo> &newRoles);

    Q_REQUIRED_RESULT QVector<RoleInfo> roles() const;

private:
    Q_DISABLE_COPY(AdminRolesModel)
    QVector<RoleInfo> mListRoleInfos;
};
