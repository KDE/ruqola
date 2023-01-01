/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorroomseditbasewidget.h"
#include "administratorroomsselectroomtypewidget.h"
#include "libruqolawidgets_private_export.h"
#include "misc/searchtreebasewidget.h"
#include "model/adminroomsfilterproxymodel.h"
#include "room.h"
#include "rooms/saveroomsettingsjob.h"
class QWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorRoomsWidget : public SearchTreeBaseWidget
{
    Q_OBJECT
public:
    explicit AdministratorRoomsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AdministratorRoomsWidget() override;

protected:
    void slotLoadElements(int offset = -1, int count = -1, const QString &searchName = {}) override;
    void slotCustomContextMenuRequested(const QPoint &pos) override;
    void updateLabel() override;

private:
    Q_REQUIRED_RESULT QString displayShowMessageInRoom() const;
    void slotDeleteGroupsDone(const QString &identifier);
    void slotDeletechannelDone(const QString &identifier);
    void slotDeleteDirectMessageDone(const QString &identifier);
    void slotFilterChanged(AdminRoomsFilterProxyModel::FilterRooms filters);
    void slotModifyRoom(const QModelIndex &index);
    void slotRemoveRoom(const QModelIndex &parentIndex);
    void slotSaveRoomSettingsDone(const QString &roomId);
    void slotGetRoomSettingsDone(const QJsonObject &obj);
    void slotGetRoomSettingsModifyDone(const QJsonObject &obj, Room::RoomType roomType);
    Q_REQUIRED_RESULT RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo
    convertToSaveRoomSettingsInfo(const AdministratorRoomsEditBaseWidget::RoomEditInfo &info, const Room::RoomType roomType, const QString &roomIdentifier);

    AdministratorRoomsSelectRoomTypeWidget *const mSelectRoomType;
};
