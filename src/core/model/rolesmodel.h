/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#pragma once

#include "libruqolacore_export.h"
#include "roleinfo.h"
#include <QAbstractListModel>
#include <QVector>
class LIBRUQOLACORE_EXPORT RolesModel : public QAbstractListModel
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

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void setRoles(const QVector<RoleInfo> &newRoles);

    Q_REQUIRED_RESULT const QVector<RoleInfo> &roles() const;

    Q_REQUIRED_RESULT const QStringList &rolesSelected() const;
    void setRolesSelected(const QStringList &newRolesSelected);

private:
    Q_DISABLE_COPY(RolesModel)
    QStringList mRolesSelected;
    QVector<RoleInfo> mRoles;
};
