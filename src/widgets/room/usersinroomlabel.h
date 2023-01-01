/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QLabel>
#include <QPointer>
#include <QWidget>
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
    QPointer<Room> mRoom;
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
    QLabel *const mIconLabel;
    UserLabel *const mUserNameLabel;
};
Q_DECLARE_METATYPE(UsersInRoomLabel::UserInfo)
Q_DECLARE_TYPEINFO(UsersInRoomLabel::UserInfo, Q_MOVABLE_TYPE);
