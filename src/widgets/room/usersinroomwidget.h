/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QPointer>
#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QLineEdit;
class Room;
class UsersInRoomMenu;
class QLabel;
class UsersInRoomComboBox;
class RocketChatAccount;
class UsersInRoomTreeView;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UsersInRoomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UsersInRoomWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~UsersInRoomWidget() override;
    void setRoom(Room *room);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void updateLabel();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextChanged(const QString &str);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCustomContextMenuRequested(const QPoint &pos);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString displayShowMessageInRoom() const;
    LIBRUQOLAWIDGETS_NO_EXPORT void loadMoreUsers();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotChangeStatusType(int index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotShowUserInfo(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotDoubleClick(const QModelIndex &index);
    UsersInRoomTreeView *const mListView;
    QLineEdit *const mSearchLineEdit;
    QPointer<Room> mRoom;
    UsersInRoomMenu *mMenu = nullptr;
    QLabel *const mMessageListInfo;
    UsersInRoomComboBox *const mUsersInRoomComboBox;
    RocketChatAccount *const mRocketChatAccount;
};
