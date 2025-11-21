/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channellistview.h"
using namespace Qt::Literals::StringLiterals;

#include "channellistdelegate.h"
#include "connection.h"
#include "dialogs/configurenotificationdialog.h"
#include "model/roomfilterproxymodel.h"
#include "model/roomlistheadingsproxymodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "subscriptions/markroomasunreadjob.h"
#include "teams/channelsconverttoteamjob.h"
#include "teams/groupsconverttoteamjob.h"
#include "teams/searchteamdialog.h"
#include "teams/teamaddroomsjob.h"
#include "teams/teamconverttochanneldialog.h"
#include "teams/teamconverttochanneljob.h"
#include "teams/teamroom.h"
#include "teams/teamslistroomsjob.h"

#include "channels/channelclosejob.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QAction>
#include <QContextMenuEvent>
#include <QList>
#include <QMenu>
#include <QPointer>

ChannelListView::ChannelListView(QWidget *parent)
    : QTreeView(parent)
    , mChannelListDelegate(new ChannelListDelegate(this))
    , mRoomListHeadingsProxyModel(new RoomListHeadingsProxyModel(this))
    , mRoomFilterProxyModel(new RoomFilterProxyModel(this))
{
    mChannelListDelegate->setObjectName(u"mChannelListDelegate"_s);
    mRoomFilterProxyModel->setObjectName(u"mRoomFilterProxyModel"_s);
    setItemDelegate(mChannelListDelegate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mRoomFilterProxyModel->setSourceModel(mRoomListHeadingsProxyModel);
    setModel(mRoomFilterProxyModel);
    setHeaderHidden(true);
    setRootIsDecorated(false);
    setItemsExpandable(false);
    setIndentation(0);

    connect(selectionModel(), &QItemSelectionModel::currentChanged, this, &ChannelListView::slotClicked);
    connect(model(), &QAbstractItemModel::rowsInserted, this, &QTreeView::expandAll);
    connect(model(), &QAbstractItemModel::modelReset, this, &QTreeView::expandAll);
    connect(model(), &QAbstractItemModel::rowsMoved, this, &QTreeView::expandAll);
    connect(model(), &QAbstractItemModel::layoutChanged, this, &QTreeView::expandAll);
    connect(this, &QTreeView::pressed, this, &ChannelListView::slotPressed);
}

ChannelListView::~ChannelListView() = default;

void ChannelListView::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::roomRemoved, this, &ChannelListView::slotRoomRemoved);
        disconnect(mUpdateChannelViewConnect);
    }
    mCurrentRocketChatAccount = currentRocketChatAccount;
    connect(mCurrentRocketChatAccount, &RocketChatAccount::roomRemoved, this, &ChannelListView::slotRoomRemoved);
    mUpdateChannelViewConnect = connect(mCurrentRocketChatAccount, &RocketChatAccount::ownUserUiPreferencesChanged, this, [this]() {
        filterModel()->setSortOrder(mCurrentRocketChatAccount->roomListSortOrder());
        mChannelListDelegate->setListDisplay(mCurrentRocketChatAccount->roomListDisplay());
    });
    filterModel()->setSortOrder(mCurrentRocketChatAccount->roomListSortOrder());
    mChannelListDelegate->setCurrentRocketChatAccount(currentRocketChatAccount);
    mChannelListDelegate->setListDisplay(mCurrentRocketChatAccount->roomListDisplay());
    mRoomListHeadingsProxyModel->setSourceModel(currentRocketChatAccount->roomModel());
}

RoomFilterProxyModel *ChannelListView::filterModel() const
{
    return mRoomFilterProxyModel;
}

void ChannelListView::slotPressed(const QModelIndex &index)
{
    if (index.isValid()) {
        if (!index.parent().isValid())
            return;

        const QByteArray roomId = index.data(RoomModel::RoomId).toByteArray();
        Q_EMIT roomPressed(roomId);
    }
}

void ChannelListView::slotClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        channelSelected(index);
    }
}

void ChannelListView::contextMenuEvent(QContextMenuEvent *event)
{
    if (mCurrentRocketChatAccount->offlineMode()) {
        return;
    }
    const QModelIndex qmi = indexAt(event->pos());
    if (!qmi.isValid()) {
        return;
    }
    if (!qmi.parent().isValid()) {
        return;
    }
    QMenu menu(this);
    // Use QPersistentModelIndex as model might have changed by the time an action is triggered
    // and will crash us if we use QModelIndex
    QPersistentModelIndex index = qmi;

    const auto roomType = index.data(RoomModel::RoomType).value<Room::RoomType>();

    const bool isUnRead = index.data(RoomModel::RoomAlert).toBool();
    const QString actionMarkAsText = isUnRead ? i18nc("@action", "Mark as Read") : i18nc("@action", "Mark as Unread");
    auto markAsChannel = new QAction(QIcon::fromTheme(u"checkmark-symbolic"_s), actionMarkAsText, &menu);
    connect(markAsChannel, &QAction::triggered, this, [this, index, isUnRead]() {
        if (index.isValid()) {
            slotMarkAsChannel(index, isUnRead);
        }
    });
    menu.addAction(markAsChannel);

    const bool isFavorite = index.data(RoomModel::RoomFavorite).toBool();
    const QString actionFavoriteText = isFavorite ? i18nc("@action", "Unset as Favorite") : i18nc("@action", "Set as Favorite");
    auto favoriteAction = new QAction(QIcon::fromTheme(u"favorite"_s), actionFavoriteText, &menu);
    connect(favoriteAction, &QAction::triggered, this, [this, index, isFavorite]() {
        if (index.isValid()) {
            slotChangeFavorite(index, isFavorite);
        }
    });
    menu.addAction(favoriteAction);

    auto hideChannel = new QAction(QIcon::fromTheme(u"hide_table_row"_s), i18nc("@action", "Hide Channel"), &menu);
    connect(hideChannel, &QAction::triggered, this, [this, index, roomType]() {
        if (index.isValid()) {
            slotHideChannel(index, roomType);
        }
    });

    if (roomType == Room::RoomType::Channel || roomType == Room::RoomType::Private) { // Not direct channel
        const QByteArray roomId = index.data(RoomModel::RoomId).toByteArray();
        Room *room = mCurrentRocketChatAccount->room(roomId);
        if (mCurrentRocketChatAccount->teamEnabled()) {
            if (room) {
                const bool mainTeam = index.data(RoomModel::RoomTeamIsMain).toBool();
                if (!mainTeam) {
                    const QByteArray mainTeamId = index.data(RoomModel::RoomTeamId).toByteArray();
                    if (mainTeamId.isEmpty() && room->hasPermission(u"convert-team"_s)) {
                        menu.addSeparator();
                        auto convertToTeam = new QAction(i18nc("@action", "Convert to Team"), &menu);
                        connect(convertToTeam, &QAction::triggered, this, [this, index, roomType]() {
                            if (index.isValid()) {
                                slotConvertToTeam(index, roomType);
                            }
                        });
                        menu.addAction(convertToTeam);
                    }
                } else {
                    if (room->hasPermission("edit-team-channel"_L1)) {
                        menu.addSeparator();
                        auto convertToChanne = new QAction(i18nc("@action", "Convert to Channel"), &menu);
                        connect(convertToChanne, &QAction::triggered, this, [this, index]() {
                            if (KMessageBox::questionTwoActions(this,
                                                                i18n("Do you want to convert team to channel?"),
                                                                i18nc("@title:window", "Convert To Channel"),
                                                                KStandardGuiItem::ok(),
                                                                KStandardGuiItem::cancel())
                                == KMessageBox::ButtonCode::SecondaryAction) {
                                return;
                            }
                            if (index.isValid()) {
                                slotConvertToChannel(index);
                            }
                        });
                        menu.addAction(convertToChanne);
                    }
                }
                const QByteArray mainTeamId = index.data(RoomModel::RoomTeamId).toByteArray();
                if (mainTeamId.isEmpty() && !mainTeam && (room->hasPermission(u"add-team-channel"_s) || room->hasPermission(u"move-room-to-team"_s))) {
                    menu.addSeparator();
                    auto moveToTeam = new QAction(i18nc("@action", "Move to Team"), &menu);
                    connect(moveToTeam, &QAction::triggered, this, [this, index]() {
                        if (index.isValid()) {
                            slotMoveToTeam(index);
                        }
                    });
                    menu.addAction(moveToTeam);
                }
            }
        }

        menu.addSeparator();
        menu.addAction(hideChannel);

        if (room) {
            menu.addSeparator();
            auto configureNotificationChannel = new QAction(QIcon::fromTheme(u"notifications-symbolic"_s), i18nc("@action", "Configure Notification…"), &menu);
            connect(configureNotificationChannel, &QAction::triggered, this, [this, room]() {
                slotConfigureNotification(room);
            });
            menu.addAction(configureNotificationChannel);
        }
        menu.addSeparator();
        auto quitChannel = new QAction(QIcon::fromTheme(u"dialog-close"_s), i18nc("@action", "Quit Channel"), &menu);
        connect(quitChannel, &QAction::triggered, this, [this, index, roomType]() {
            if (index.isValid()) {
                slotLeaveChannel(index, roomType);
            }
        });
        menu.addAction(quitChannel);
    } else {
        menu.addSeparator();
        menu.addAction(hideChannel);
    }
    if (!menu.actions().isEmpty()) {
        menu.exec(event->globalPos());
    }
}

void ChannelListView::slotConfigureNotification(Room *room)
{
    if (!room) {
        return;
    }
    ConfigureNotificationDialog dlg(mCurrentRocketChatAccount, this);
    dlg.setRoom(room);
    dlg.exec();
}

void ChannelListView::slotMoveToTeam(const QModelIndex &index)
{
    QPointer<SearchTeamDialog> dlg = new SearchTeamDialog(mCurrentRocketChatAccount, this);
    if (dlg->exec()) {
        const QByteArray teamId = dlg->teamId();
        if (!teamId.isEmpty()) {
            auto job = new RocketChatRestApi::TeamAddRoomsJob(this);
            job->setTeamId(teamId);
            const QByteArray roomId = index.data(RoomModel::RoomId).toByteArray();
            job->setRoomIds({roomId});

            mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamAddRoomsJob job";
            }
        } else {
            KMessageBox::information(this, i18n("Any team selected."), i18nc("@title:window", "Move To Team"));
        }
    }
    delete dlg;
}

void ChannelListView::slotConvertToChannel(const QModelIndex &index)
{
    const QByteArray teamId = index.data(RoomModel::RoomTeamId).toByteArray();
    auto job = new RocketChatRestApi::TeamsListRoomsJob(this);
    job->setTeamId(teamId);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::TeamsListRoomsJob::teamListRoomsDone, this, [this, teamId, index](const QJsonObject &obj) {
        const QList<TeamRoom> teamRooms = TeamRoom::parseTeamRooms(obj);
        QList<QByteArray> listRoomIdToDelete;
        if (!teamRooms.isEmpty()) {
            QPointer<TeamConvertToChannelDialog> dlg = new TeamConvertToChannelDialog(this);
            const QString teamName = index.data(RoomModel::RoomName).toString();
            dlg->setTeamName(teamName);
            dlg->setTeamRooms(teamRooms);
            if (dlg->exec()) {
                listRoomIdToDelete = dlg->roomIdsToDelete();
            } else {
                delete dlg;
                return;
            }
            delete dlg;
        }
        auto job = new RocketChatRestApi::TeamConvertToChannelJob(this);
        job->setTeamId(teamId);
        job->setRoomsToRemove(listRoomIdToDelete);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        // connect(job, &RocketChatRestApi::TeamConvertToChannelJob::teamConvertToChannelDone, this, []() {
        //     // TODO ?
        // });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamConvertToChannelJob job";
        }
    });

    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamsListRoomsJob job";
    }
}

void ChannelListView::slotConvertToTeam(const QModelIndex &index, Room::RoomType roomType)
{
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
                                           i18n("Are you sure to convert it to team? It can not be undo."),
                                           i18nc("@title:window", "Convert to Team"),
                                           KStandardGuiItem::ok(),
                                           KStandardGuiItem::cancel())) {
        const QString roomId = index.data(RoomModel::RoomId).toString();
        switch (roomType) {
        case Room::RoomType::Unknown:
            qCWarning(RUQOLAWIDGETS_LOG) << "Unknown type used it's a bug";
            break;
        case Room::RoomType::Direct:
            qCWarning(RUQOLAWIDGETS_LOG) << "We can't convert Direct to Team. It's a bug";
            break;
        case Room::RoomType::Channel: {
            auto job = new RocketChatRestApi::ChannelsConvertToTeamJob(this);
            job->setChannelId(roomId);
            mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ChannelsConvertToTeamJob job";
            }
            break;
        }
        case Room::RoomType::Private: {
            auto job = new RocketChatRestApi::GroupsConvertToTeamJob(this);
            job->setRoomId(roomId);
            mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ChannelsConvertToTeamJob job";
            }
            break;
        }
        }
    }
}

void ChannelListView::slotMarkAsChannel(const QModelIndex &index, bool markAsRead)
{
    const QByteArray roomId = index.data(RoomModel::RoomId).toByteArray();
    if (markAsRead) {
        mCurrentRocketChatAccount->markRoomAsRead(roomId);
    } else {
        auto job = new RocketChatRestApi::MarkRoomAsUnReadJob(this);
        job->setObjectId(roomId);
        job->setUnReadObject(RocketChatRestApi::MarkRoomAsUnReadJob::MarkAsUnReadObject::Room);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start markRoomAsUnRead job";
        }
    }
}

void ChannelListView::channelSelected(const QModelIndex &index)
{
    if (!index.parent().isValid())
        return;

    const QByteArray roomId = index.data(RoomModel::RoomId).toByteArray();
    const QString roomName = index.data(RoomModel::RoomFName).toString();
    const auto roomType = index.data(RoomModel::RoomType).value<Room::RoomType>();
    const auto avatarInfo = index.data(RoomModel::RoomAvatarInfo).value<Utils::AvatarInfo>();
    ChannelSelectedInfo info;
    info.avatarInfo = avatarInfo;
    info.roomId = roomId;
    info.roomName = roomName;
    info.roomType = roomType;
    Q_EMIT roomSelected(info);
}

void ChannelListView::slotHideChannel(const QModelIndex &index, Room::RoomType roomType)
{
    const QByteArray roomId = index.data(RoomModel::RoomId).toByteArray();
    auto job = new RocketChatRestApi::ChannelCloseJob(this);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo info;
    info.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
    info.identifier = QString::fromLatin1(roomId);
    job->setChannelGroupInfo(info);
    const QString type = Room::roomFromRoomType(roomType);
    if (type == u'd') {
        job->setChannelType(RocketChatRestApi::ChannelCloseJob::ChannelType::Direct);
    } else if (type == u'p') {
        job->setChannelType(RocketChatRestApi::ChannelCloseJob::ChannelType::Groups);
    } else if (type == u'c') {
        job->setChannelType(RocketChatRestApi::ChannelCloseJob::ChannelType::Channel);
    }
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ChannelCloseJob job";
    }
}

void ChannelListView::slotLeaveChannel(const QModelIndex &index, Room::RoomType roomType)
{
    const QByteArray roomId = index.data(RoomModel::RoomId).toByteArray();
    mCurrentRocketChatAccount->leaveRoom(roomId, roomType);
}

void ChannelListView::slotChangeFavorite(const QModelIndex &index, bool isFavorite)
{
    const QByteArray roomId = index.data(RoomModel::RoomId).toByteArray();
    mCurrentRocketChatAccount->changeFavorite(roomId, !isFavorite);
}

void ChannelListView::selectChannelRequested(const QByteArray &channelId, const QByteArray &messageId)
{
    if (!selectChannelByRoomIdRequested(QString::fromLatin1(channelId))) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Room not found:" << channelId;
        mCurrentRocketChatAccount->delaySelectChannelRequested(channelId);
    } else {
        if (!messageId.isEmpty()) {
            Q_EMIT selectMessageIdRequested(messageId);
        }
    }
}

bool ChannelListView::selectChannelByRoomIdOrRoomName(const QString &id, bool roomId)
{
    if (id.isEmpty()) {
        return false;
    }
    Q_ASSERT(filterModel());
    const int nSections = filterModel()->rowCount();
    for (int sectionId = 0; sectionId < nSections; ++sectionId) {
        const auto section = filterModel()->index(sectionId, 0, {});
        const auto sectionSize = filterModel()->rowCount(section);

        for (int roomIdx = 0; roomIdx < sectionSize; ++roomIdx) {
            const auto roomModelIndex = filterModel()->index(roomIdx, 0, section);
            const auto identifier =
                roomId ? QString::fromLatin1(roomModelIndex.data(RoomModel::RoomId).toByteArray()) : roomModelIndex.data(RoomModel::RoomName).toString();
            if (identifier == id) {
                selectionModel()->setCurrentIndex(roomModelIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
                return true;
            }
        }
    }
    return false;
}

bool ChannelListView::selectChannelByRoomNameRequested(const QString &selectedRoomName)
{
    return selectChannelByRoomIdOrRoomName(selectedRoomName, false);
}

bool ChannelListView::selectChannelByRoomIdRequested(const QString &identifier)
{
    return selectChannelByRoomIdOrRoomName(identifier, true);
}

void ChannelListView::selectNextUnreadChannel()
{
    selectNextChannel(Direction::Down, true);
}

void ChannelListView::selectNextChannel(Direction direction, bool switchToNextUnreadChannel)
{
    Q_ASSERT(filterModel());

    const auto nSections = filterModel()->rowCount();
    if (nSections == 0) {
        // FIXME : switch to empty room widget ?
        return;
    }

    const QModelIndex initialIndex = selectionModel()->currentIndex();
    QModelIndex currentIndex = initialIndex;

    // nextIndex(invalid) → top or bottom
    // nextIndex(other) → above or below, invalid on overflow
    const auto nextIndex = [this, direction](const QModelIndex &index) {
        if (!index.isValid()) {
            switch (direction) {
            case Direction::Up: {
                QModelIndex lastIndex = filterModel()->index(filterModel()->rowCount() - 1, 0);
                while (filterModel()->rowCount(lastIndex) > 0) {
                    lastIndex = filterModel()->index(filterModel()->rowCount(lastIndex) - 1, 0, lastIndex);
                }
                return lastIndex;
            }
            case Direction::Down:
                return filterModel()->index(0, 0, {});
            }
            Q_UNREACHABLE();
        }

        switch (direction) {
        case Direction::Up: {
            return indexAbove(index);
        }
        case Direction::Down: {
            return indexBelow(index);
        }
        }
        Q_UNREACHABLE();
    };

    const auto matchesFilter = [switchToNextUnreadChannel](const QModelIndex &index) {
        return index.isValid() && index.flags().testFlag(Qt::ItemIsSelectable) && (!switchToNextUnreadChannel || index.data(RoomModel::RoomAlert).toBool());
    };

    do {
        currentIndex = nextIndex(currentIndex);
    } while (currentIndex != initialIndex && !matchesFilter(currentIndex));

    if (currentIndex.isValid()) {
        selectionModel()->setCurrentIndex(currentIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }
}

void ChannelListView::slotRoomRemoved(const QByteArray &roomId)
{
    const auto currentlySelectedIndex = selectionModel()->currentIndex();
    const QByteArray currentRoomId = currentlySelectedIndex.data(RoomModel::RoomId).toByteArray();
    if (currentRoomId == roomId) {
        selectNextChannel();
    }
}

#include "moc_channellistview.cpp"
