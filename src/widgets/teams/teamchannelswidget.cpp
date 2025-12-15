/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamchannelswidget.h"

#include "channels/createchanneljob.h"
#include "connection.h"
#include "dialogs/createnewchanneldialog.h"
#include "groups/creategroupsjob.h"
#include "model/teamroomsfilterproxymodel.h"
#include "model/teamroomsmodel.h"
#include "rocketchataccount.h"
#include "ruqolaserverconfig.h"
#include "ruqolawidgets_debug.h"
#include "teams/teamaddroomsjob.h"
#include "teams/teamchannelscombobox.h"
#include "teams/teamremoveroomjob.h"
#include "teams/teamroom.h"
#include "teams/teamsearchroomdialog.h"
#include "teams/teamslistroomsjob.h"
#include "teams/teamupdateroomjob.h"
#include <KLineEditEventHandler>

#include <KLocalizedString>
#include <KMessageBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QMenu>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
TeamChannelsWidget::TeamChannelsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mListView(new QListView(this))
    , mSearchLineEdit(new QLineEdit(this))
    , mTeamChannelsCombobox(new TeamChannelsComboBox(this))
    , mTeamRoomsModel(new TeamRoomsModel(this))
    , mTeamRoomFilterProxyModel(new TeamRoomsFilterProxyModel(mTeamRoomsModel, this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(u"hboxLayout"_s);
    hboxLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(u"mSearchLineEdit"_s);
    hboxLayout->addWidget(mSearchLineEdit);
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &TeamChannelsWidget::slotTextChanged);

    mTeamChannelsCombobox->setObjectName(u"mTeamChannelsCombobox"_s);
    hboxLayout->addWidget(mTeamChannelsCombobox);

    mainLayout->addLayout(hboxLayout);

    mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search room…"));
    mSearchLineEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mSearchLineEdit);

    mListView->setObjectName(u"mListView"_s);
    mainLayout->addWidget(mListView);

    mListView->setModel(mTeamRoomFilterProxyModel);
    connect(mTeamChannelsCombobox, &TeamChannelsComboBox::currentIndexChanged, this, &TeamChannelsWidget::slotTypeTeamListChanged);
    mListView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mListView, &QListView::customContextMenuRequested, this, &TeamChannelsWidget::slotCustomContextMenuRequested);
}

TeamChannelsWidget::~TeamChannelsWidget() = default;

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
    auto job = new RocketChatRestApi::TeamsListRoomsJob(this);
    job->setTeamId(mTeamId);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::TeamsListRoomsJob::teamListRoomsDone, this, &TeamChannelsWidget::slotTeamListRoomsDone);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamsListRoomsJob job";
    }
}

void TeamChannelsWidget::slotTeamListRoomsDone(const QJsonObject &obj)
{
    const QList<TeamRoom> teamRooms = TeamRoom::parseTeamRooms(obj);
    mTeamRoomsModel->setTeamRooms(std::move(teamRooms));
}

void TeamChannelsWidget::slotTextChanged(const QString &str)
{
    mTeamRoomFilterProxyModel->setFilterString(str);
}

void TeamChannelsWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    QModelIndex index = mListView->indexAt(pos);
    // TODO Remove add-team-channel when we will depend against RC 7.0
    if (mRoom->hasPermission(u"add-team-channel"_s) || mRoom->hasPermission(u"move-room-to-team"_s) || mRoom->hasPermission(u"create-team-channel"_s)
        || mRoom->hasPermission(u"create-team-group"_s)) {
        menu.addAction(i18n("Add Existing Room"), this, &TeamChannelsWidget::slotAddExistingRoom);
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(u"irc-join-channel"_s), i18nc("@action", "Create Room"), this, &TeamChannelsWidget::slotCreateRoom);
    }
    if (index.isValid()) {
        if (mRoom->hasPermission(u"edit-team-channel"_s)) {
            menu.addSeparator();
            const bool autojoin = index.data(TeamRoomsModel::AutoJoin).toBool();
            menu.addAction(autojoin ? i18nc("@action", "Remove Autojoin") : i18nc("@action", "Add Autojoin"), this, [this, index, autojoin]() {
                const QByteArray roomId = index.data(TeamRoomsModel::Identifier).toByteArray();
                updateAutojoin(roomId, autojoin);
            });
        }
        if (mRoom->hasPermission(u"remove-team-channel"_s)) {
            menu.addSeparator();
            menu.addAction(QIcon::fromTheme(u"dialog-cancel"_s), i18nc("@action", "Remove from Team"), this, [this, index]() {
                const QByteArray roomId = index.data(TeamRoomsModel::Identifier).toByteArray();
                removeRoomFromTeam(roomId);
            });
        }
    }
    if (!menu.isEmpty()) {
        menu.exec(mListView->viewport()->mapToGlobal(pos));
    }
}

void TeamChannelsWidget::updateAutojoin(const QByteArray &roomId, bool autojoin)
{
    auto job = new RocketChatRestApi::TeamUpdateRoomJob(this);
    job->setIsDefault(!autojoin);
    job->setRoomId(roomId);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::TeamUpdateRoomJob::teamUpdateRoomDone, this, &TeamChannelsWidget::slotTeamUpdateRoomDone);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamUpdateRoomJob job";
    }
}

void TeamChannelsWidget::slotTeamUpdateRoomDone(const QJsonObject &replyObject)
{
    const QJsonObject room = replyObject.value("room"_L1).toObject();
    TeamRoom teamRoom;
    teamRoom.parse(room);
    mTeamRoomsModel->setRoomChanged(std::move(teamRoom));
}

void TeamChannelsWidget::removeRoomFromTeam(const QByteArray &roomId)
{
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
                                           i18n("Would you like to remove this Channel from team?"),
                                           i18nc("@title", "Remove Channel from Team"),
                                           KStandardGuiItem::remove(),
                                           KStandardGuiItem::cancel())) {
        auto job = new RocketChatRestApi::TeamRemoveRoomJob(this);
        job->setTeamId(mTeamId);
        job->setRoomId(roomId);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
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
    QList<QByteArray> roomIds;
    QPointer<TeamSearchRoomDialog> dlg = new TeamSearchRoomDialog(mRocketChatAccount, this);
    if (dlg->exec()) {
        roomIds = dlg->roomIds();
    }
    delete dlg;
    if (!roomIds.isEmpty()) {
        auto job = new RocketChatRestApi::TeamAddRoomsJob(this);
        job->setTeamId(mTeamId);
        job->setRoomIds(roomIds);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::TeamAddRoomsJob::teamAddRoomsDone, this, &TeamChannelsWidget::slotTeamAddRoomsDone);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamAddRoomsJob job";
        }
    }
}

void TeamChannelsWidget::slotTeamAddRoomsDone(const QJsonObject &obj)
{
    // qDebug() << " obj " << obj;
    const QList<TeamRoom> teamRooms = TeamRoom::parseTeamRooms(obj);
    if (!teamRooms.isEmpty()) {
        mTeamRoomsModel->insertRooms(teamRooms);
    }
}

void TeamChannelsWidget::slotCreateRoom()
{
    QPointer<CreateNewChannelDialog> dlg = new CreateNewChannelDialog(mRocketChatAccount, this);
    CreateNewChannelWidget::Features flags;
    if (mRocketChatAccount->ruqolaServerConfig()->broadCastEnabled()) {
        flags |= CreateNewChannelWidget::Feature::BroadCast;
    }
    if (mRocketChatAccount->ruqolaServerConfig()->encryptionEnabled()) {
        flags |= CreateNewChannelWidget::Feature::Encrypted;
    }
    if (mRocketChatAccount->ruqolaServerConfig()->federationEnabled()) {
        flags |= CreateNewChannelWidget::Feature::Federated;
    }
    dlg->setFeatures(flags);
    if (dlg->exec()) {
        RocketChatRestApi::CreateChannelTeamInfo createRoominfo = dlg->channelInfo(false);
        createRoominfo.teamId = mTeamId;
        createRoominfo.infoType = RocketChatRestApi::CreateChannelTeamInfo::CreateInfoType::Channel;
        if (createRoominfo.privateChannel) {
            createGroups(createRoominfo);
        } else {
            createChannels(createRoominfo);
        }
    }
    delete dlg;
}

void TeamChannelsWidget::createChannels(const RocketChatRestApi::CreateChannelTeamInfo &info)
{
    auto job = new RocketChatRestApi::CreateChannelJob(this);
    // TODO connect(job, &RocketChatRestApi::CreateChannelJob::addJoinCodeToChannel, this, &RestApiConnection::slotAddJoinCodeToChannel);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::CreateChannelJob::createChannelDone, this, [this](const QJsonObject &replyObject) {
        const QJsonObject obj = replyObject["channel"_L1].toObject();
        TeamRoom teamRoom;
        teamRoom.parse(obj);
        mTeamRoomsModel->insertRooms({teamRoom});
    });
    job->setCreateChannelInfo(info);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start CreateChannelJob job";
    }
}

void TeamChannelsWidget::createGroups(const RocketChatRestApi::CreateChannelTeamInfo &info)
{
    auto job = new RocketChatRestApi::CreateGroupsJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    job->setCreateGroupsInfo(info);
    connect(job, &RocketChatRestApi::CreateGroupsJob::createGroupsDone, this, [this](const QJsonObject &replyObject) {
        const QJsonObject obj = replyObject["group"_L1].toObject();
        TeamRoom teamRoom;
        teamRoom.parse(obj);
        mTeamRoomsModel->insertRooms({teamRoom});
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start CreateGroupsJob job";
    }
}

#include "moc_teamchannelswidget.cpp"
