/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>
class QWidget;

#include "libruqolawidgets_private_export.h"
class UsersInRoomWidget;
class RocketChatAccount;
class Room;
class LIBRUQOLAWIDGETS_TESTS_EXPORT UsersInRoomDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UsersInRoomDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~UsersInRoomDialog() override;

    void setRoom(Room *room);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    UsersInRoomWidget *const mUsersInRoomWidget;
};
