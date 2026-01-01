/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "teams/teamroomcompleter.h"
#include "utils.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT TeamRoomCompleterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    // TeamRoomCompleter
    enum TeamRoomCompleterRoles {
        TeamName = Qt::UserRole + 1,
        TeamId,
        AvatarInfo,
    };
    Q_ENUM(TeamRoomCompleterRoles)

    explicit TeamRoomCompleterModel(QObject *parent = nullptr);
    ~TeamRoomCompleterModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();
    void setRooms(const QList<TeamRoomCompleter> &rooms);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT Utils::AvatarInfo avatarInfo(const TeamRoomCompleter &room) const;
    QList<TeamRoomCompleter> mRooms;
};
