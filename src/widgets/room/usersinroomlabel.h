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

#ifndef USERSINROOMLABEL_H
#define USERSINROOMLABEL_H

#include <QLabel>
#include <QWidget>
#include "libruqolawidgets_private_export.h"
class Room;
class UsersInRoomMenu;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UserLabel : public QLabel
{
    Q_OBJECT
public:
    explicit UserLabel(QWidget *parent = nullptr);
    ~UserLabel() override;
    void setRoom(Room *room);
    void setUserId(const QString &userId);
    void setUserName(const QString &userName);
private:
    void slotCustomContextMenuRequested(const QPoint &pos);
    void slotOpenConversation();
    QString mUserId;
    QString mUserName;
    Room *mRoom = nullptr;
    UsersInRoomMenu *mMenu = nullptr;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT UsersInRoomLabel : public QWidget
{
    Q_OBJECT
public:
    struct UserInfo {
        QString userName;
        QString userId;
        QString iconStatus;
        QString userDisplayName;
    };
    explicit UsersInRoomLabel(QWidget *parent = nullptr);
    ~UsersInRoomLabel() override;

    void setUserInfo(const UsersInRoomLabel::UserInfo &info);
    void setRoom(Room *room);
private:
    UsersInRoomLabel::UserInfo mInfo;
    QLabel *mIconLabel = nullptr;
    UserLabel *mUserNameLabel = nullptr;
};
Q_DECLARE_METATYPE(UsersInRoomLabel::UserInfo)
Q_DECLARE_TYPEINFO(UsersInRoomLabel::UserInfo, Q_MOVABLE_TYPE);
#endif // USERSINROOMLABEL_H
