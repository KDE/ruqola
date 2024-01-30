/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

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
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString displayShowMessageInRoom() const;
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDeleteGroupsDone(const QString &identifier);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDeletechannelDone(const QString &identifier);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDeleteDirectMessageDone(const QString &identifier);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFilterChanged(AdminRoomsFilterProxyModel::FilterRooms filters);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotModifyRoom(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemoveRoom(const QModelIndex &parentIndex);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSaveRoomSettingsDone(const QString &roomId);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotGetRoomSettingsDone(const QJsonObject &obj);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotGetRoomSettingsModifyDone(const QJsonObject &obj, Room::RoomType roomType);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo
    convertToSaveRoomSettingsInfo(const AdministratorRoomsEditBaseWidget::RoomEditInfo &info, const Room::RoomType roomType, const QString &roomIdentifier);

    AdministratorRoomsSelectRoomTypeWidget *const mSelectRoomType;
};
