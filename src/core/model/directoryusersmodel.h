/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#pragma once

#include "directorybasemodel.h"
#include "libruqolacore_export.h"
class LIBRUQOLACORE_EXPORT DirectoryUsersModel : public DirectoryBaseModel
{
    Q_OBJECT
public:
    explicit DirectoryUsersModel(QObject *parent = nullptr);
    ~DirectoryUsersModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;
    Q_REQUIRED_RESULT QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void parseElements(const QJsonObject &discussionsObj) override;
    void addMoreElements(const QJsonObject &obj) override;

private:
    Q_DISABLE_COPY(DirectoryUsersModel)
    void checkFullList();
};
