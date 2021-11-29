/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDialog>

#include "libruqolawidgets_private_export.h"
class TeamSearchRoomWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TeamSearchRoomDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TeamSearchRoomDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~TeamSearchRoomDialog() override;

    Q_REQUIRED_RESULT QStringList roomIds() const;

private:
    void readConfig();
    void writeConfig();
    TeamSearchRoomWidget *const mTeamSearchRoomWidget;
};
