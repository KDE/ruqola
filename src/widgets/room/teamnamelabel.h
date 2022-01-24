/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "model/roommodel.h"
#include <QLabel>

#include "libruqolawidgets_private_export.h"
class LIBRUQOLAWIDGETS_TESTS_EXPORT TeamNameLabel : public QLabel
{
    Q_OBJECT
public:
    struct TeamNameInfo {
        QString teamName;
        QString teamIdentifier;
    };

    explicit TeamNameLabel(QWidget *parent = nullptr);
    ~TeamNameLabel() override;

    void setTeamRoomInfo(const Room::TeamRoomInfo &newTeamNameInfo);

Q_SIGNALS:
    void openTeam(const QString &identifier);

private:
    void slotGoToRoomTeam(const QString &contents);
    Room::TeamRoomInfo mTeamNameInfo;
};
