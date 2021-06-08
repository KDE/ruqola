/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "administratorroomsselectroomtypewidget.h"
#include "libruqolawidgets_private_export.h"
#include "misc/searchtreebasewidget.h"
#include "model/adminroomsfilterproxymodel.h"
#include <QWidget>
class AdminRoomsFilterProxyModel;
class AdministratorRoomsSelectRoomTypeWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorRoomsWidget : public SearchTreeBaseWidget
{
    Q_OBJECT
public:
    explicit AdministratorRoomsWidget(QWidget *parent = nullptr);
    ~AdministratorRoomsWidget() override;

protected:
    void slotLoadElements(int offset = -1, int count = -1, const QString &searchName = {}) override;
    void slotCustomContextMenuRequested(const QPoint &pos) override;
    void updateLabel() override;
    void addExtraWidget(QVBoxLayout *layout) override;

private:
    Q_REQUIRED_RESULT QString displayShowMessageInRoom() const;
    void slotFilterChanged(AdminRoomsFilterProxyModel::FilterRooms filters);
    void slotTextChanged(const QString &text);
    void slotAddRoom();
    void slotModifyRoom(const QModelIndex &index);
    void slotRemoveRoom(const QModelIndex &index);
    AdministratorRoomsSelectRoomTypeWidget *const mSelectRoomType;
    AdminRoomsFilterProxyModel *mAdminRoomsProxyModel = nullptr;
};

