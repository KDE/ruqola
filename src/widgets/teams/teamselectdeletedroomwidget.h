/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "teams/teamroom.h"
#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QListView;
class QLineEdit;
class TeamRoomsModel;
class TeamRoomsFilterProxyModel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TeamSelectDeletedRoomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TeamSelectDeletedRoomWidget(QWidget *parent = nullptr);
    ~TeamSelectDeletedRoomWidget() override;
    [[nodiscard]] QStringList roomsId() const;

    void setTeamRooms(const QVector<TeamRoom> &rooms);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextChanged(const QString &str);
    QListView *const mListView;
    QLineEdit *const mSearchLineEdit;
    TeamRoomsModel *const mTeamRoomsModel;
    TeamRoomsFilterProxyModel *const mTeamSelectProxyModel;
};
