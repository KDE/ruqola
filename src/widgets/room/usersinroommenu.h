/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
#include <QPointer>

#include "libruqolawidgets_private_export.h"
class Room;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UsersInRoomMenu : public QObject
{
    Q_OBJECT
public:
    explicit UsersInRoomMenu(QObject *parent = nullptr);
    ~UsersInRoomMenu() override;

    void slotCustomContextMenuRequested(const QPoint &pos);
    void setRoom(Room *room);
    void setUserName(const QString &userName);
    void setUserId(const QString &userId);
    void setParentWidget(QWidget *parentWidget);

private:
    void slotOpenConversation();
    void slotBlockUser();
    void slotIgnoreUser();
    void slotRemoveFromRoom();
    void slotUserInfo();
    QString mUserId;
    QString mUserName;
    QPointer<Room> mRoom;
    QWidget *mParentWidget = nullptr;
};
