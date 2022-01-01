/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QMap>
#include <QWidget>
class FlowLayout;
class Room;
class UsersInRoomLabel;
class UsersForRoomFilterProxyModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UsersInRoomFlowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UsersInRoomFlowWidget(QWidget *parent = nullptr);
    ~UsersInRoomFlowWidget() override;
    void setRoom(Room *room);

protected:
    void showEvent(QShowEvent *event) override;

private:
    void loadMoreUsersAttachment();
    void generateListUsersWidget();
    void updateListUsersWidget(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void loadExternalDialog();
    FlowLayout *const mFlowLayout;
    Room *mRoom = nullptr;
    // userId, UsersInRoomLabel
    QMap<QString, UsersInRoomLabel *> mListUsersWidget;
    UsersForRoomFilterProxyModel *const mUsersForRoomFilterProxyModel;
};

