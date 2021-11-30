/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UsersInRoomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UsersInRoomWidget(RocketChatAccount *account, QWidget *parent = nullptr);
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
    QLabel *const mMessageListInfo;
    UsersForRoomFilterProxyModel *const mUsersForRoomFilterProxy;
    UsersInRoomComboBox *const mUsersInRoomComboBox;
    RocketChatAccount *const mRocketChatAccount;
};

