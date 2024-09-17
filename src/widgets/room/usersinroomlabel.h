/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QLabel>
#include <QPointer>
class Room;
class UsersInRoomMenu;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UserLabel : public QLabel
{
    Q_OBJECT
public:
    explicit UserLabel(QWidget *parent = nullptr);
    ~UserLabel() override;
    void setRoom(Room *room);
    void setUserId(const QByteArray &userId);
    void setUserName(const QString &userName);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCustomContextMenuRequested(const QPoint &pos);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOpenConversation();
    QByteArray mUserId;
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
        QByteArray userId;
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
Q_DECLARE_TYPEINFO(UsersInRoomLabel::UserInfo, Q_RELOCATABLE_TYPE);
