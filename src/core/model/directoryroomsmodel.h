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
#include "roomsinfo.h"
class LIBRUQOLACORE_EXPORT DirectoryRoomsModel : public DirectoryBaseModel
{
    Q_OBJECT
public:
    enum DirectoryRoomsRoles {
        Name,
        Topic,
        UsersCount,
        ReadOnly,
        DefaultRoom,
        Identifier,
        ChannelType,
        ChannelTypeStr,
        LastMessage,
        LastColumn = LastMessage,
    };
    Q_ENUM(DirectoryRoomsRoles)

    explicit DirectoryRoomsModel(QObject *parent = nullptr);
    ~DirectoryRoomsModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;
    Q_REQUIRED_RESULT QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_REQUIRED_RESULT int columnCount(const QModelIndex &parent) const override;
    Q_REQUIRED_RESULT int total() const override;

    void parseElements(const QJsonObject &roomsObj) override;

    void addMoreElements(const QJsonObject &discussionsObj) override;

protected:
    Q_REQUIRED_RESULT QList<int> hideColumns() const override;

private:
    Q_DISABLE_COPY(DirectoryRoomsModel)
    void checkFullList();
    RoomsInfo mRoomsInfo;
};
