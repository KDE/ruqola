/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "createchannelteaminfo.h"
#include <QPointer>
#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QListView;
class QLineEdit;
class TeamRoomsModel;
class TeamRoomsFilterProxyModel;
class TeamChannelsComboBox;
class Room;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TeamChannelsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TeamChannelsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
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
    void createChannels(const RocketChatRestApi::CreateChannelTeamInfo &info);
    void createGroups(const RocketChatRestApi::CreateChannelTeamInfo &info);
    QListView *const mListView;
    QLineEdit *const mSearchLineEdit;
    TeamChannelsComboBox *const mTeamChannelsCombobox;
    TeamRoomsModel *const mTeamRoomsModel;
    TeamRoomsFilterProxyModel *const mTeamRoomFilterProxyModel;
    QPointer<Room> mRoom;
    RocketChatAccount *const mRocketChatAccount;
    QString mTeamId;
};
