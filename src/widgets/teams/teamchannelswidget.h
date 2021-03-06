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

#include "createroominfo.h"
#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QListView;
class QLineEdit;
class TeamRoomsModel;
class TeamRoomsFilterProxyModel;
class TeamChannelsComboBox;
class Room;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TeamChannelsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TeamChannelsWidget(QWidget *parent = nullptr);
    ~TeamChannelsWidget() override;

    void setRoom(Room *room);

private:
    void slotTeamListRoomsDone(const QJsonObject &obj);
    void initializeTeamRoomsList();
    void slotTypeTeamListChanged(int index);
    void slotTextChanged(const QString &str);
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotAddExistingRoom();
    void slotCreateRoom();
    void slotRemoveTeamRoomDone();
    void slotTeamUpdateRoomDone(const QJsonObject &replyObject);
    void removeRoomFromTeam(const QString &roomId);
    void updateAutojoin(const QString &roomId, bool autojoin);
    void slotTeamAddRoomsDone(const QJsonObject &obj);
    void createChannels(const RocketChatRestApi::CreateRoomInfo &info);
    void createGroups(const RocketChatRestApi::CreateRoomInfo &info);
    QListView *const mListView;
    QLineEdit *const mSearchLineEdit;
    TeamChannelsComboBox *const mTeamChannelsCombobox;
    TeamRoomsModel *const mTeamRoomsModel;
    TeamRoomsFilterProxyModel *const mTeamRoomFilterProxyModel;
    Room *mRoom = nullptr;
    QString mTeamId;
};
