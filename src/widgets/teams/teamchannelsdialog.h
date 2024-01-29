/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>

#include "libruqolawidgets_private_export.h"
class TeamChannelsWidget;
class Room;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TeamChannelsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TeamChannelsDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~TeamChannelsDialog() override;
    void setRoom(Room *room);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    TeamChannelsWidget *const mTeamChannelsWidget;
};
