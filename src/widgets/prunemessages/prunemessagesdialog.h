/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>

#include "rooms/roomscleanhistoryjob.h"

#include "libruqolawidgets_private_export.h"
class PruneMessagesWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PruneMessagesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PruneMessagesDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~PruneMessagesDialog() override;
    [[nodiscard]] RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo cleanHistoryInfo() const;

    void setRoomName(const QString &roomName);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    PruneMessagesWidget *const mPruneMessageWidget;
};
