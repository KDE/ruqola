/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>

#include "libruqolawidgets_private_export.h"
class TeamConvertToChannelWidget;
class TeamRoom;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TeamConvertToChannelDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TeamConvertToChannelDialog(QWidget *parent = nullptr);
    ~TeamConvertToChannelDialog() override;

    [[nodiscard]] QStringList roomIdsToDelete() const;
    void setTeamRooms(const QVector<TeamRoom> &rooms);

    void setTeamName(const QString &name);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    TeamConvertToChannelWidget *const mTeamConvertToChannelWidget;
};
