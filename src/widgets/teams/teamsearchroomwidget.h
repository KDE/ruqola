/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class TeamSearchRoomForTeamWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TeamSearchRoomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TeamSearchRoomWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~TeamSearchRoomWidget() override;

    [[nodiscard]] QList<QByteArray> roomIds() const;

Q_SIGNALS:
    void enableOkButton(bool enabled);

private:
    TeamSearchRoomForTeamWidget *const mTeamSearchRoomForTeamWidget;
};
