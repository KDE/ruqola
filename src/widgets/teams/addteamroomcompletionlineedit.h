/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

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
        QByteArray roomId;
        QString roomName;
    };
    explicit AddTeamRoomCompletionLineEdit(RocketChatAccount *account, QWidget *parent = nullptr);
    ~AddTeamRoomCompletionLineEdit() override;

Q_SIGNALS:
    void newRoomName(const AddTeamRoomCompletionLineEdit::RoomCompletionInfo &);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchTimerFired();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchTextEdited();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextChanged(const QString &text);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotComplete(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAutoCompletTeamRoomDone(const QJsonObject &obj);
    TeamRoomCompleterModel *const mTeamRoomCompleterModel;
    QTimer *const mSearchTimer;
    RocketChatAccount *const mRocketChatAccount;
};
