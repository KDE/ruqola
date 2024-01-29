/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class AddUsersInRoomWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AddUsersInRoomDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddUsersInRoomDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AddUsersInRoomDialog() override;
    [[nodiscard]] QStringList userIds() const;
    [[nodiscard]] QStringList userNames() const;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    AddUsersInRoomWidget *const mAddUsersInRoomWidget;
};
