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
#include "teams/teamroom.h"
#include <QAbstractListModel>
#include <QVector>
class LIBRUQOLACORE_EXPORT TeamRoomsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum TeamRoomsRoles {
        Name = Qt::UserRole + 1,
        AutoJoin,
        Identifier,
    };
    Q_ENUM(TeamRoomsRoles)

    explicit TeamRoomsModel(QObject *parent = nullptr);
    ~TeamRoomsModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    Q_REQUIRED_RESULT QVector<TeamRoom> teamRooms() const;
    void setTeamRooms(const QVector<TeamRoom> &teamRooms);

    void setRoomChanged(const TeamRoom &teamRoom);

    void insertRooms(const QVector<TeamRoom> &teamRooms);

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    Q_REQUIRED_RESULT bool isCheckable() const;
    void setIsCheckable(bool isCheckable);

private:
    Q_DISABLE_COPY(TeamRoomsModel)
    QVector<TeamRoom> mTeamRooms;
    QStringList mRoomSelected;
    bool mIsCheckable = false;
};
