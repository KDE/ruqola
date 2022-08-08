/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QMap>
#include <QPointer>
#include <QWidget>
class FlowLayout;
class Room;
class UsersInRoomLabel;
class UsersForRoomFilterProxyModel;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UsersInRoomFlowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UsersInRoomFlowWidget(QWidget *parent = nullptr);
    ~UsersInRoomFlowWidget() override;
    void setRoom(Room *room);

    void setCurrentRocketChatAccount(RocketChatAccount *account);

protected:
    void showEvent(QShowEvent *event) override;

private:
    void loadMoreUsersAttachment();
    void generateListUsersWidget();
    void updateListUsersWidget(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void loadExternalDialog();
    FlowLayout *const mFlowLayout;
    QPointer<Room> mRoom;
    // userId, UsersInRoomLabel
    QMap<QString, UsersInRoomLabel *> mListUsersWidget;
    UsersForRoomFilterProxyModel *const mUsersForRoomFilterProxyModel;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};
