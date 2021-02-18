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

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QListView;
class QLineEdit;
class Room;
class UsersInRoomMenu;
class QLabel;
class UsersForRoomFilterProxyModel;
class UsersInRoomComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UsersInRoomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UsersInRoomWidget(QWidget *parent = nullptr);
    ~UsersInRoomWidget() override;
    void setRoom(Room *room);

private:
    void updateLabel();
    void slotTextChanged(const QString &str);
    void slotCustomContextMenuRequested(const QPoint &pos);
    Q_REQUIRED_RESULT QString displayShowMessageInRoom() const;
    void loadMoreUsers();
    void slotChangeStatusType(int index);
    void slotShowUserInfo(const QModelIndex &index);
    QListView *const mListView;
    QLineEdit *const mSearchLineEdit;
    Room *mRoom = nullptr;
    UsersInRoomMenu *mMenu = nullptr;
    QLabel *const mMessageListInfo = nullptr;
    UsersForRoomFilterProxyModel *const mUsersForRoomFilterProxy;
    UsersInRoomComboBox *const mUsersInRoomComboBox;
};

