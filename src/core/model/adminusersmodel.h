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

#include "directoryusersmodel.h"
#include <QVector>

#include "libruqolacore_export.h"
#include "user.h"

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
        LastColumn = ActiveUser,
    };
    Q_ENUM(AdminUsersRoles)

    explicit AdminUsersModel(QObject *parent = nullptr);
    ~AdminUsersModel() override;

    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;
    Q_REQUIRED_RESULT QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_REQUIRED_RESULT int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QList<int> hideColumns() const override;
    Q_REQUIRED_RESULT bool setData(const QModelIndex &index, const QVariant &value, int role) override;

protected:
    Q_REQUIRED_RESULT Users::ParseType parseType() const override;
    void removeElement(const QString &identifier) override;
};

