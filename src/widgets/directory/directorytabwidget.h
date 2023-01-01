/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QTabWidget>

#include "libruqolawidgets_private_export.h"
class DirectoryWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT DirectoryTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit DirectoryTabWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~DirectoryTabWidget() override;

    void fillTabs();

private:
    DirectoryWidget *const mRooms;
    DirectoryWidget *const mUsers;
    DirectoryWidget *const mTeams;
};
