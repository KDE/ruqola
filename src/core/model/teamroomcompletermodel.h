/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "teams/teamroomcompleter.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT TeamRoomCompleterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    // TeamRoomCompleter
    enum TeamRoomCompleterRoles {
        TeamName = Qt::UserRole + 1,
        TeamId,
        TeamIcon,
    };
    Q_ENUM(TeamRoomCompleterRoles)

    explicit TeamRoomCompleterModel(QObject *parent = nullptr);
    ~TeamRoomCompleterModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();
    void insertRooms(const QVector<TeamRoomCompleter> &rooms);

private:
    Q_DISABLE_COPY(TeamRoomCompleterModel)
    QVector<TeamRoomCompleter> mRooms;
};
