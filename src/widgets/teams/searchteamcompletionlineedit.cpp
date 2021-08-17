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

#include "searchteamcompletionlineedit.h"
#include "common/completionlistview.h"
#include "model/teamcompletermodel.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "teamroomcompleter.h"
#include "teams/teamsautocompletejob.h"

#include <QJsonArray>
#include <QTimer>
#include <chrono>

using namespace std::chrono_literals;

SearchTeamCompletionLineEdit::SearchTeamCompletionLineEdit(QWidget *parent)
    : CompletionLineEdit(parent)
    , mTeamCompleterModel(new TeamCompleterModel(this))
    , mSearchTimer(new QTimer(this))
{
    setCompletionModel(mTeamCompleterModel);
    connect(this, &SearchTeamCompletionLineEdit::complete, this, &SearchTeamCompletionLineEdit::slotComplete);

    connect(mSearchTimer, &QTimer::timeout, this, &SearchTeamCompletionLineEdit::slotSearchTimerFired);
    connect(this, &QLineEdit::textChanged, this, &SearchTeamCompletionLineEdit::slotSearchTextEdited);
}

SearchTeamCompletionLineEdit::~SearchTeamCompletionLineEdit()
{
}

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
    } else {
        auto *rcAccount = Ruqola::self()->rocketChatAccount();
        auto job = new RocketChatRestApi::TeamsAutoCompleteJob(this);

        job->setName(text);
        rcAccount->restApi()->initializeRestApiJob(job);
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
    for (int i = 0, total = items.count(); i < total; ++i) {
        TeamCompleter teamCompleter;
        teamCompleter.parse(items.at(i).toObject());
        teams.append(teamCompleter);
    }
    mTeamCompleterModel->insertRooms(teams);
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
}
