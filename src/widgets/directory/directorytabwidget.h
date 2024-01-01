/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QTabWidget>

#include "libruqolawidgets_private_export.h"
class RocketChatAccount;
class DirectoryStackedWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT DirectoryTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit DirectoryTabWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~DirectoryTabWidget() override;

    void fillTabs();

private:
    DirectoryStackedWidget *const mRooms;
    DirectoryStackedWidget *const mUsers;
    DirectoryStackedWidget *const mTeams;
};
