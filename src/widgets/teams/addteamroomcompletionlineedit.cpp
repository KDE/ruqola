/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "addteamroomcompletionlineedit.h"
#include "common/completionlistview.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "teams/roomsautocompleteavailableforteamsjob.h"

AddTeamRoomCompletionLineEdit::AddTeamRoomCompletionLineEdit(QWidget *parent)
    : CompletionLineEdit(parent)
{
    connect(this, &QLineEdit::textChanged, this, &AddTeamRoomCompletionLineEdit::slotTextChanged);
    // setCompletionModel(Ruqola::self()->rocketChatAccount()->userCompleterFilterModelProxy());
    connect(this, &AddTeamRoomCompletionLineEdit::complete, this, &AddTeamRoomCompletionLineEdit::slotComplete);
}

AddTeamRoomCompletionLineEdit::~AddTeamRoomCompletionLineEdit()
{
}

QStringList AddTeamRoomCompletionLineEdit::roomIds() const
{
    return {};
}

void AddTeamRoomCompletionLineEdit::slotTextChanged(const QString &text)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto job = new RocketChatRestApi::RoomsAutocompleteAvailableForTeamsJob(this);

    RocketChatRestApi::RoomsAutocompleteAvailableForTeamsJob::RoomsAutocompleteChannelAndPrivateInfo info;
    info.name = text;
    job->setRoomsCompleterInfo(info);
    rcAccount->restApi()->initializeRestApiJob(job);
    connect(job,
            &RocketChatRestApi::RoomsAutocompleteAvailableForTeamsJob::roomsAutoCompleteChannelAndPrivateDone,
            this,
            &AddTeamRoomCompletionLineEdit::slotRemoveTeamRoomDone);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamsListRoomsJob job";
    }
}

void AddTeamRoomCompletionLineEdit::slotRemoveTeamRoomDone(const QJsonObject &obj)
{
    qDebug() << " obj " << obj;
    // TODO
}

void AddTeamRoomCompletionLineEdit::slotComplete(const QModelIndex &index)
{
#if 0
    const QString completerName = index.data(UserCompleterModel::UserName).toString();
    const QString userId = index.data(UserCompleterModel::UserId).toString();
    UserCompletionInfo info;
    info.username = completerName;
    info.userId = userId;
    mCompletionListView->hide();
    disconnect(this, &QLineEdit::textChanged, this, &AddTeamRoomCompletionLineEdit::slotTextChanged);
    Q_EMIT newUserName(info);
    clear();
    connect(this, &QLineEdit::textChanged, this, &AddTeamRoomCompletionLineEdit::slotTextChanged);
#endif
}
