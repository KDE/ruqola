/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchteamcompletionlineedit.h"
#include "common/completionlistview.h"
#include "connection.h"
#include "model/teamcompletermodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "teams/teamsautocompletejob.h"

#include <QJsonArray>
#include <QTimer>
#include <chrono>

using namespace std::chrono_literals;

SearchTeamCompletionLineEdit::SearchTeamCompletionLineEdit(RocketChatAccount *account, QWidget *parent)
    : CompletionLineEdit(parent)
    , mTeamCompleterModel(new TeamCompleterModel(this))
    , mSearchTimer(new QTimer(this))
    , mRocketChatAccount(account)
{
    setCompletionModel(mTeamCompleterModel);
    connect(this, &SearchTeamCompletionLineEdit::complete, this, &SearchTeamCompletionLineEdit::slotComplete);

    connect(mSearchTimer, &QTimer::timeout, this, &SearchTeamCompletionLineEdit::slotSearchTimerFired);
    connect(this, &QLineEdit::textChanged, this, &SearchTeamCompletionLineEdit::slotSearchTextEdited);
}

SearchTeamCompletionLineEdit::~SearchTeamCompletionLineEdit() = default;

const QString &SearchTeamCompletionLineEdit::teamId() const
{
    return mTeamId;
}

void SearchTeamCompletionLineEdit::slotSearchTimerFired()
{
    mSearchTimer->stop();
    slotTextChanged(text());
}

void SearchTeamCompletionLineEdit::slotSearchTextEdited()
{
    if (mSearchTimer->isActive()) {
        mSearchTimer->stop(); // eventually
    }

    mSearchTimer->setSingleShot(true);
    mSearchTimer->start(500ms);
}

void SearchTeamCompletionLineEdit::slotTextChanged(const QString &text)
{
    if (text.trimmed().isEmpty()) {
        mTeamCompleterModel->clear();
        mTeamId.clear();
        Q_EMIT teamIdChanged(true);
    } else {
        auto job = new RocketChatRestApi::TeamsAutoCompleteJob(this);

        job->setName(text);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::TeamsAutoCompleteJob::teamAutoCompleteDone, this, &SearchTeamCompletionLineEdit::slotTeamAutoCompleteDone);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamsListRoomsJob job";
        }
    }
}

void SearchTeamCompletionLineEdit::slotTeamAutoCompleteDone(const QJsonObject &obj)
{
    // qDebug() << " obj " << obj;
    const QJsonArray items = obj[QLatin1String("teams")].toArray();
    QVector<TeamCompleter> teams;
    const auto total = items.count();
    teams.reserve(total);
    for (auto i = 0; i < total; ++i) {
        TeamCompleter teamCompleter;
        teamCompleter.parse(items.at(i).toObject());
        teams.append(std::move(teamCompleter));
    }
    mTeamCompleterModel->insertTeams(teams);
}

void SearchTeamCompletionLineEdit::slotComplete(const QModelIndex &index)
{
    const QString completerName = index.data(TeamCompleterModel::TeamName).toString();
    const QString teamIdentifier = index.data(TeamCompleterModel::TeamId).toString();
    mCompletionListView->hide();
    disconnect(this, &QLineEdit::textChanged, this, &SearchTeamCompletionLineEdit::slotSearchTextEdited);
    setText(completerName);
    mTeamId = teamIdentifier;
    connect(this, &QLineEdit::textChanged, this, &SearchTeamCompletionLineEdit::slotSearchTextEdited);
    Q_EMIT teamIdChanged(mTeamId.isEmpty());
}
