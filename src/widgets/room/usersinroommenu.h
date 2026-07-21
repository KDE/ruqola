/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
#include <QPointer>

#include "libruqolawidgets_private_export.h"
class Room;
class QMenu;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UsersInRoomMenu : public QObject
{
    Q_OBJECT
public:
    explicit UsersInRoomMenu(RocketChatAccount *account, QObject *parent = nullptr);
    ~UsersInRoomMenu() override;

    void slotCustomContextMenuRequested(const QPoint &pos);
    void setRoom(Room *room);
    void setUserName(const QString &userName);
    void setUserId(const QByteArray &userId);
    void setParentWidget(QWidget *parentWidget);

    void updateUserInRoomMenu();

    [[nodiscard]] QMenu *createUserInRoomMenu();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOpenConversation();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotBlockUser();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotIgnoreUser();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRemoveFromRoom();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUserInfo();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotReportUser();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotMuteUser();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotBanUserFromRoomAction();
    QByteArray mUserId;
    QString mUserName;
    QPointer<Room> mRoom;
    QWidget *mParentWidget = nullptr;
    RocketChatAccount *const mRocketChatAccount;
    QMenu *mMenu = nullptr;
};
