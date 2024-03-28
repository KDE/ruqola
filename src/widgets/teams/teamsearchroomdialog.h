/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

    [[nodiscard]] QList<QByteArray> roomIds() const;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    TeamSearchRoomWidget *const mTeamSearchRoomWidget;
};
