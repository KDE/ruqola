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
#include "channels/createchanneljob.h"
#include "connection.h"
#include "dialogs/createnewchanneldialog.h"
#include "groups/creategroupsjob.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/teamroomsfilterproxymodel.h"
#include "model/teamroomsmodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "teams/teamaddroomsjob.h"
#include "teams/teamchannelscombobox.h"
#include "teams/teamremoveroomjob.h"
#include "teams/teamroom.h"
#include "teams/teamsearchroomdialog.h"
#include "teams/teamslistroomsjob.h"
#include "teams/teamupdateroomjob.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QMenu>
#include <QPointer>
#include <QVBoxLayout>

TeamChannelsWidget::TeamChannelsWidget(QWidget *parent)
    : QWidget(parent)
    , mListView(new QListView(this))
    , mSearchLineEdit(new QLineEdit(this))
    , mTeamChannelsCombobox(new TeamChannelsComboBox(this))
    , mTeamRoomsModel(new TeamRoomsModel(this))
    , mTeamRoomFilterProxyModel(new TeamRoomsFilterProxyModel(mTeamRoomsModel, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    hboxLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    hboxLayout->addWidget(mSearchLineEdit);
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &TeamChannelsWidget::slotTextChanged);

    mTeamChannelsCombobox->setObjectName(QStringLiteral("mTeamChannelsCombobox"));
    hboxLayout->addWidget(mTeamChannelsCombobox);

    mainLayout->addLayout(hboxLayout);

    mSearchLineEdit->setPlaceholderText(i18n("Search Room..."));
    mSearchLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchLineEdit, this);

    mListView->setObjectName(QStringLiteral("mListView"));
    mainLayout->addWidget(mListView);

    mListView->setModel(mTeamRoomFilterProxyModel);
    connect(mTeamChannelsCombobox, &TeamChannelsComboBox::currentIndexChanged, this, &TeamChannelsWidget::slotTypeTeamListChanged);
    mListView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mListView, &QListView::customContextMenuRequested, this, &TeamChannelsWidget::slotCustomContextMenuRequested);
}

TeamChannelsWidget::~TeamChannelsWidget()
{
}

void TeamChannelsWidget::slotTypeTeamListChanged(int index)
{
    mTeamRoomFilterProxyModel->setSortByAutoJoin(index == 1 ? true : false);
}

void TeamChannelsWidget::setRoom(Room *room)
{
    mRoom = room;
    mTeamId = mRoom->teamInfo().teamId();
    initializeTeamRoomsList();
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
    const QVector<TeamRoom> teamRooms = TeamRoom::parseTeamRooms(obj);
    mTeamRoomsModel->setTeamRooms(teamRooms);
}

void TeamChannelsWidget::slotTextChanged(const QString &str)
{
    mTeamRoomFilterProxyModel->setFilterString(str);
}

void TeamChannelsWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    QModelIndex index = mListView->indexAt(pos);
    if (mRoom->hasPermission(QStringLiteral("add-team-channel"))) {
        menu.addAction(i18n("Add Existing Room"), this, &TeamChannelsWidget::slotAddExistingRoom);
        menu.addSeparator();
        menu.addAction(i18n("Create Room"), this, &TeamChannelsWidget::slotCreateRoom);
    }
    if (index.isValid()) {
        if (mRoom->hasPermission(QStringLiteral("edit-team-channel"))) {
            menu.addSeparator();
            const bool autojoin = index.data(TeamRoomsModel::AutoJoin).toBool();
            menu.addAction(autojoin ? i18n("Remove Autojoin") : i18n("Add Autojoin"), this, [this, index, autojoin]() {
                const QString roomId = index.data(TeamRoomsModel::Identifier).toString();
                updateAutojoin(roomId, autojoin);
            });
        }
        if (mRoom->hasPermission(QStringLiteral("remove-team-channel"))) {
            menu.addSeparator();
            menu.addAction(QIcon::fromTheme(QStringLiteral("dialog-cancel")), i18n("Remove from Team"), this, [this, index]() {
                const QString roomId = index.data(TeamRoomsModel::Identifier).toString();
                removeRoomFromTeam(roomId);
            });
        }
    }
    if (!menu.isEmpty()) {
        menu.exec(mListView->viewport()->mapToGlobal(pos));
    }
}

void TeamChannelsWidget::updateAutojoin(const QString &roomId, bool autojoin)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto job = new RocketChatRestApi::TeamUpdateRoomJob(this);
    job->setIsDefault(!autojoin);
    job->setRoomId(roomId);
    rcAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::TeamUpdateRoomJob::teamUpdateRoomDone, this, &TeamChannelsWidget::slotTeamUpdateRoomDone);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamUpdateRoomJob job";
    }
}

void TeamChannelsWidget::slotTeamUpdateRoomDone(const QJsonObject &replyObject)
{
    const QJsonObject room = replyObject.value(QLatin1String("room")).toObject();
    TeamRoom teamRoom;
    teamRoom.parse(room);
    mTeamRoomsModel->setRoomChanged(teamRoom);
}

void TeamChannelsWidget::removeRoomFromTeam(const QString &roomId)
{
    if (KMessageBox::Yes
        == KMessageBox::questionYesNo(this, i18n("Would you like to remove this Channel from team?"), i18nc("@title", "Remove Channel from Team"))) {
        auto *rcAccount = Ruqola::self()->rocketChatAccount();
        auto job = new RocketChatRestApi::TeamRemoveRoomJob(this);
        job->setTeamId(mTeamId);
        job->setRoomId(roomId);
        rcAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::TeamRemoveRoomJob::removeTeamRoomDone, this, &TeamChannelsWidget::slotRemoveTeamRoomDone);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamsListRoomsJob job";
        }
    }
}

void TeamChannelsWidget::slotRemoveTeamRoomDone()
{
    // TODO try to remove item without sending new rest api call
    initializeTeamRoomsList();
}

void TeamChannelsWidget::slotAddExistingRoom()
{
    QStringList roomIds;
    QPointer<TeamSearchRoomDialog> dlg = new TeamSearchRoomDialog(this);
    if (dlg->exec()) {
        roomIds = dlg->roomIds();
    }
    delete dlg;
    if (!roomIds.isEmpty()) {
        auto *rcAccount = Ruqola::self()->rocketChatAccount();
        auto job = new RocketChatRestApi::TeamAddRoomsJob(this);
        job->setTeamId(mTeamId);
        job->setRoomIds(roomIds);
        rcAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::TeamAddRoomsJob::teamAddRoomsDone, this, &TeamChannelsWidget::slotTeamAddRoomsDone);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamAddRoomsJob job";
        }
    }
}

void TeamChannelsWidget::slotTeamAddRoomsDone(const QJsonObject &obj)
{
    // qDebug() << " obj " << obj;
    const QVector<TeamRoom> teamRooms = TeamRoom::parseTeamRooms(obj);
    if (!teamRooms.isEmpty()) {
        mTeamRoomsModel->insertRooms(teamRooms);
    }
}

void TeamChannelsWidget::slotCreateRoom()
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    QPointer<CreateNewChannelDialog> dlg = new CreateNewChannelDialog(this);
    CreateNewChannelWidget::Features flags;
    if (rcAccount->broadCastEnabled()) {
        flags |= CreateNewChannelWidget::Feature::BroadCast;
    }
    if (rcAccount->encryptionEnabled()) {
        flags |= CreateNewChannelWidget::Feature::Encrypted;
    }
    dlg->setFeatures(flags);
    if (dlg->exec()) {
        const CreateNewChannelDialog::NewChannelInfo info = dlg->channelInfo();
        RocketChatRestApi::CreateRoomInfo createRoominfo = info.info;
        createRoominfo.teamId = mTeamId;
        if (info.privateChannel) {
            createGroups(createRoominfo);
        } else {
            createChannels(createRoominfo);
        }
    }
    delete dlg;
}

void TeamChannelsWidget::createChannels(const RocketChatRestApi::CreateRoomInfo &info)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    auto job = new RocketChatRestApi::CreateChannelJob(this);
    // TODO connect(job, &RocketChatRestApi::CreateChannelJob::addJoinCodeToChannel, this, &RestApiConnection::slotAddJoinCodeToChannel);
    rcAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::CreateChannelJob::createChannelDone, this, [this](const QJsonObject &replyObject) {
        const QJsonObject obj = replyObject[QLatin1String("channel")].toObject();
        TeamRoom teamRoom;
        teamRoom.parse(obj);
        mTeamRoomsModel->insertRooms({teamRoom});
    });
    job->setCreateChannelInfo(info);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start CreateChannelJob job";
    }
}

void TeamChannelsWidget::createGroups(const RocketChatRestApi::CreateRoomInfo &info)
{
    auto job = new RocketChatRestApi::CreateGroupsJob(this);
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    rcAccount->restApi()->initializeRestApiJob(job);
    job->setCreateGroupsInfo(info);
    connect(job, &RocketChatRestApi::CreateGroupsJob::createGroupsDone, this, [this](const QJsonObject &replyObject) {
        const QJsonObject obj = replyObject[QLatin1String("group")].toObject();
        TeamRoom teamRoom;
        teamRoom.parse(obj);
        mTeamRoomsModel->insertRooms({teamRoom});
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start CreateGroupsJob job";
    }
}
