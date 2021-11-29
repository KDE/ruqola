/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
