/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>

#include "libruqolawidgets_private_export.h"
class ConfigureNotificationWidget;
class Room;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureNotificationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigureNotificationDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ConfigureNotificationDialog() override;

    void setRoom(Room *room);

private:
    ConfigureNotificationWidget *const mConfigureNoticationWidget;
};
