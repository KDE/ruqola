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

#include <QObject>

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
    Room *mRoom = nullptr;
    QWidget *mParentWidget = nullptr;
};

