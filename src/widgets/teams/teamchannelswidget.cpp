/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

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

    mSearchLineEdit->setPlaceholderText(i18n("Search room..."));
    mSearchLineEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mSearchLineEdit);

    mListView->setObjectName(QStringLiteral("mListView"));
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
    if (mRoom->hasPermission(QStringLiteral("add-team-channel"))) {
        menu.addAction(i18n("Add Existing Room"), this, &TeamChannelsWidget::slotAddExistingRoom);
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("irc-join-channel")), i18n("Create Room"), this, &TeamChannelsWidget::slotCreateRoom);
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
    const QJsonObject room = replyObject.value(QLatin1String("room")).toObject();
    TeamRoom teamRoom;
    teamRoom.parse(room);
    mTeamRoomsModel->setRoomChanged(std::move(teamRoom));
}

void TeamChannelsWidget::removeRoomFromTeam(const QString &roomId)
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
    QStringList roomIds;
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
    if (mRocketChatAccount->broadCastEnabled()) {
        flags |= CreateNewChannelWidget::Feature::BroadCast;
    }
    if (mRocketChatAccount->encryptionEnabled()) {
        flags |= CreateNewChannelWidget::Feature::Encrypted;
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

void TeamChannelsWidget::createGroups(const RocketChatRestApi::CreateChannelTeamInfo &info)
{
    auto job = new RocketChatRestApi::CreateGroupsJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
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

#include "moc_teamchannelswidget.cpp"
