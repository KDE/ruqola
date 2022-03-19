/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class AddUsersWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateDirectMessagesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CreateDirectMessagesWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~CreateDirectMessagesWidget() override;
    Q_REQUIRED_RESULT QStringList userNames() const;
Q_SIGNALS:
    void updateOkButton(bool state);

private:
    AddUsersWidget *const mUsers;
};
