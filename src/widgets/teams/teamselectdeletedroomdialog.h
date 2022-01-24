/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "teams/teamroom.h"
#include <QDialog>
class TeamSelectDeletedRoomWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TeamSelectDeletedRoomDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TeamSelectDeletedRoomDialog(QWidget *parent = nullptr);
    ~TeamSelectDeletedRoomDialog() override;

    void setTeamRooms(const QVector<TeamRoom> &rooms);

    Q_REQUIRED_RESULT QStringList roomsId() const;

private:
    void readConfig();
    void writeConfig();
    TeamSelectDeletedRoomWidget *const mTeamSelectDeletedRoomWidget;
};
