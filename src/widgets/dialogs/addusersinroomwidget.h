/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class AddUsersWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AddUsersInRoomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddUsersInRoomWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AddUsersInRoomWidget() override;

    Q_REQUIRED_RESULT QStringList userIds() const;

    Q_REQUIRED_RESULT QStringList userNames() const;
Q_SIGNALS:
    void updateOkButton(bool state);

private:
    AddUsersWidget *const mAddUsersWidget;
};
