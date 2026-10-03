/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "teams/teamroom.h"
#include <QWidget>

#include "libruqolawidgets_private_export.h"
class TeamSelectDeletedRoomWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TeamConvertToChannelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TeamConvertToChannelWidget(QWidget *parent = nullptr);
    ~TeamConvertToChannelWidget() override;
    [[nodiscard]] QList<QByteArray> roomIdsToDelete() const;
    void setTeamRooms(QList<TeamRoom> rooms);

private:
    TeamSelectDeletedRoomWidget *const mTeamSelectDeletedRoomWidget;
};
