/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "common/completionlineedit.h"
#include "libruqolawidgets_private_export.h"
class TeamRoomCompleterModel;
class QTimer;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AddTeamRoomCompletionLineEdit : public CompletionLineEdit
{
    Q_OBJECT
public:
    struct RoomCompletionInfo {
        QString roomId;
        QString roomName;
    };
    explicit AddTeamRoomCompletionLineEdit(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AddTeamRoomCompletionLineEdit() override;

Q_SIGNALS:
    void newRoomName(const AddTeamRoomCompletionLineEdit::RoomCompletionInfo &);

private:
    void slotSearchTimerFired();
    void slotSearchTextEdited();
    void slotTextChanged(const QString &text);
    void slotComplete(const QModelIndex &index);
    void slotAutoCompletTeamRoomDone(const QJsonObject &obj);
    TeamRoomCompleterModel *const mTeamRoomCompleterModel;
    QTimer *const mSearchTimer;
    RocketChatAccount *const mRocketChatAccount;
};
