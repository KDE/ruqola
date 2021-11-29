/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QDialog>

#include "rooms/roomscleanhistoryjob.h"

#include "libruqolawidgets_private_export.h"
class PruneMessagesWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PruneMessagesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PruneMessagesDialog(QWidget *parent = nullptr);
    ~PruneMessagesDialog() override;
    Q_REQUIRED_RESULT RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo cleanHistoryInfo() const;

    void setRoomName(const QString &roomName);

private:
    void readConfig();
    void writeConfig();
    PruneMessagesWidget *const mPruneMessageWidget;
};

