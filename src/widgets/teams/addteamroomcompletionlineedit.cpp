/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "addteamroomcompletionlineedit.h"
#include "common/completionlistview.h"
#include "connection.h"
#include "model/teamroomcompletermodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "teams/roomsautocompleteavailableforteamsjob.h"
#include "teams/teamroomcompleter.h"

#include <KLocalizedString>

#include <QJsonArray>
#include <QTimer>
#include <chrono>

using namespace std::chrono_literals;

AddTeamRoomCompletionLineEdit::AddTeamRoomCompletionLineEdit(RocketChatAccount *account, QWidget *parent)
    : CompletionLineEdit(parent)
    , mTeamRoomCompleterModel(new TeamRoomCompleterModel(this))
    , mSearchTimer(new QTimer(this))
    , mRocketChatAccount(account)
{
    setPlaceholderText(i18n("Search rooms..."));
    setCompletionModel(mTeamRoomCompleterModel);
    connect(this, &AddTeamRoomCompletionLineEdit::complete, this, &AddTeamRoomCompletionLineEdit::slotComplete);

    connect(mSearchTimer, &QTimer::timeout, this, &AddTeamRoomCompletionLineEdit::slotSearchTimerFired);
    connect(this, &QLineEdit::textChanged, this, &AddTeamRoomCompletionLineEdit::slotSearchTextEdited);
}

AddTeamRoomCompletionLineEdit::~AddTeamRoomCompletionLineEdit() = default;

void AddTeamRoomCompletionLineEdit::slotSearchTimerFired()
{
    mSearchTimer->stop();
    slotTextChanged(text());
}

void AddTeamRoomCompletionLineEdit::slotSearchTextEdited()
{
    if (mSearchTimer->isActive()) {
        mSearchTimer->stop(); // eventually
    }

    mSearchTimer->setSingleShot(true);
    mSearchTimer->start(500ms);
}

void AddTeamRoomCompletionLineEdit::slotTextChanged(const QString &text)
{
    if (text.trimmed().isEmpty()) {
        mTeamRoomCompleterModel->clear();
        mCompletionListView->hide();
    } else {
        auto job = new RocketChatRestApi::RoomsAutocompleteAvailableForTeamsJob(this);

        RocketChatRestApi::RoomsAutocompleteAvailableForTeamsJob::RoomsAutocompleteChannelAndPrivateInfo info;
        info.name = text;
        job->setRoomsCompleterInfo(info);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job,
                &RocketChatRestApi::RoomsAutocompleteAvailableForTeamsJob::roomsAutoCompleteChannelAndPrivateDone,
                this,
                &AddTeamRoomCompletionLineEdit::slotAutoCompletTeamRoomDone);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamsListRoomsJob job";
        }
    }
}

void AddTeamRoomCompletionLineEdit::slotAutoCompletTeamRoomDone(const QJsonObject &obj)
{
    // qDebug() << " obj " << obj;
    const QJsonArray items = obj[QLatin1String("items")].toArray();
    QList<TeamRoomCompleter> teams;
    for (int i = 0, total = items.count(); i < total; ++i) {
        TeamRoomCompleter teamCompleter;
        teamCompleter.parse(items.at(i).toObject());
        teams.append(std::move(teamCompleter));
    }
    mTeamRoomCompleterModel->setRooms(teams);
    if (teams.isEmpty()) {
        mCompletionListView->hide();
    }
}

void AddTeamRoomCompletionLineEdit::slotComplete(const QModelIndex &index)
{
    const QString completerName = index.data(TeamRoomCompleterModel::TeamName).toString();
    const QString roomId = index.data(TeamRoomCompleterModel::TeamId).toString();
    RoomCompletionInfo info;
    info.roomName = completerName;
    info.roomId = roomId;
    mCompletionListView->hide();
    disconnect(this, &QLineEdit::textChanged, this, &AddTeamRoomCompletionLineEdit::slotSearchTextEdited);
    Q_EMIT newRoomName(std::move(info));
    clear();
    connect(this, &QLineEdit::textChanged, this, &AddTeamRoomCompletionLineEdit::slotSearchTextEdited);
}

#include "moc_addteamroomcompletionlineedit.cpp"
