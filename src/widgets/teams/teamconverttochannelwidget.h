/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class TeamSelectDeletedRoomWidget;
class TeamRoom;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TeamConvertToChannelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TeamConvertToChannelWidget(QWidget *parent = nullptr);
    ~TeamConvertToChannelWidget() override;
    Q_REQUIRED_RESULT QStringList roomIdsToDelete() const;
    void setTeamRooms(const QVector<TeamRoom> &rooms);
    void setTeamName(const QString &name);

private:
    TeamSelectDeletedRoomWidget *const mTeamSelectDeletedRoomWidget;
};
