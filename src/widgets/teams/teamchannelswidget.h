/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

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
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTeamListRoomsDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void initializeTeamRoomsList();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTypeTeamListChanged(int index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextChanged(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCustomContextMenuRequested(const QPoint &pos);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAddExistingRoom();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCreateRoom();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemoveTeamRoomDone();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTeamUpdateRoomDone(const QJsonObject &replyObject);
    LIBRUQOLAWIDGETS_NO_EXPORT void removeRoomFromTeam(const QString &roomId);
    LIBRUQOLAWIDGETS_NO_EXPORT void updateAutojoin(const QString &roomId, bool autojoin);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTeamAddRoomsDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void createChannels(const RocketChatRestApi::CreateChannelTeamInfo &info);
    LIBRUQOLAWIDGETS_NO_EXPORT void createGroups(const RocketChatRestApi::CreateChannelTeamInfo &info);
    QListView *const mListView;
    QLineEdit *const mSearchLineEdit;
    TeamChannelsComboBox *const mTeamChannelsCombobox;
    TeamRoomsModel *const mTeamRoomsModel;
    TeamRoomsFilterProxyModel *const mTeamRoomFilterProxyModel;
    QPointer<Room> mRoom;
    RocketChatAccount *const mRocketChatAccount;
    QString mTeamId;
};
