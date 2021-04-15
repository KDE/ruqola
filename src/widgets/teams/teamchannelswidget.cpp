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

#include "teamchannelswidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/teamroomsfilterproxymodel.h"
#include "model/teamroomsmodel.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "teamroom.h"
#include "teams/teamslistroomsjob.h"
#include <KLocalizedString>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>

TeamChannelsWidget::TeamChannelsWidget(QWidget *parent)
    : QWidget(parent)
    , mListView(new QListView(this))
    , mSearchLineEdit(new QLineEdit(this))
    , mTeamRoomsModel(new TeamRoomsModel(this))
    , mTeamRoomFilterProxyModel(new TeamRoomsFilterProxyModel(mTeamRoomsModel, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mainLayout->addWidget(mSearchLineEdit);

    mSearchLineEdit->setPlaceholderText(i18n("Search Room..."));
    mSearchLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchLineEdit, this);

    mListView->setObjectName(QStringLiteral("mListView"));
    mainLayout->addWidget(mListView);

    mListView->setModel(mTeamRoomFilterProxyModel);
}

TeamChannelsWidget::~TeamChannelsWidget()
{
}

void TeamChannelsWidget::setTeamId(const QString &teamId)
{
    if (mTeamId != teamId) {
        mTeamId = teamId;
        initializeTeamRoomsList();
    }
}

void TeamChannelsWidget::initializeTeamRoomsList()
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto job = new RocketChatRestApi::TeamsListRoomsJob(this);
    job->setTeamId(mTeamId);
    rcAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::TeamsListRoomsJob::teamListRoomsDone, this, &TeamChannelsWidget::slotTeamListRoomsDone);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamsListRoomsJob job";
    }
}

void TeamChannelsWidget::slotTeamListRoomsDone(const QJsonObject &obj)
{
    QVector<TeamRoom> teamRooms;
    const QJsonArray rooms = obj.value(QLatin1String("rooms")).toArray();
    for (int i = 0; i < rooms.count(); ++i) {
        QJsonObject r = rooms.at(i).toObject();
        TeamRoom teamRoom;
        teamRoom.parse(r);
        teamRooms.append(teamRoom);
        // qDebug() << "TeamRoom  " << teamRoom;
    }
    mTeamRoomsModel->setTeamRooms(teamRooms);
}
