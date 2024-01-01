/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channellistview.h"
#include "channellistdelegate.h"
#include "connection.h"
#include "dialogs/configurenotificationdialog.h"
#include "model/roomfilterproxymodel.h"
#include "model/roomlistheadingsproxymodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "teams/channelsconverttoteamjob.h"
#include "teams/groupsconverttoteamjob.h"
#include "teams/searchteamdialog.h"
#include "teams/teamaddroomsjob.h"
#include "teams/teamconverttochanneldialog.h"
#include "teams/teamconverttochanneljob.h"
#include "teams/teamroom.h"
#include "teams/teamslistroomsjob.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>
#include <QPointer>
#include <QVector>

ChannelListView::ChannelListView(QWidget *parent)
    : QTreeView(parent)
    , mChannelListDelegate(new ChannelListDelegate(this))
    , mRoomListHeadingsProxyModel(new RoomListHeadingsProxyModel(this))
    , mRoomFilterProxyModel(new RoomFilterProxyModel(this))
{
    mChannelListDelegate->setObjectName(QStringLiteral("mChannelListDelegate"));
    mRoomFilterProxyModel->setObjectName(QStringLiteral("mRoomFilterProxyModel"));
    setItemDelegate(mChannelListDelegate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mRoomFilterProxyModel->setSourceModel(mRoomListHeadingsProxyModel);
    setModel(mRoomFilterProxyModel);
    setHeaderHidden(true);
    setRootIsDecorated(false);
    setUniformRowHeights(true);
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
    mUpdateChannelViewConnect = connect(mCurrentRocketChatAccount, &RocketChatAccount::ownUserPreferencesChanged, this, [this]() {
        filterModel()->setSortOrder(mCurrentRocketChatAccount->roomListSortOrder());
    });
    filterModel()->setSortOrder(mCurrentRocketChatAccount->roomListSortOrder());
    mChannelListDelegate->setCurrentRocketChatAccount(currentRocketChatAccount);
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

        const QString roomId = index.data(RoomModel::RoomId).toString();
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
    const QString actionMarkAsText = isUnRead ? i18n("Mark As Read") : i18n("Mark As Unread");
    auto markAsChannel = new QAction(actionMarkAsText, &menu);
    connect(markAsChannel, &QAction::triggered, this, [this, index, isUnRead]() {
        if (index.isValid()) {
            slotMarkAsChannel(index, isUnRead);
        }
    });
    menu.addAction(markAsChannel);

    const bool isFavorite = index.data(RoomModel::RoomFavorite).toBool();
    const QString actionFavoriteText = isFavorite ? i18n("Unset as Favorite") : i18n("Set as Favorite");
    auto favoriteAction = new QAction(QIcon::fromTheme(QStringLiteral("favorite")), actionFavoriteText, &menu);
    connect(favoriteAction, &QAction::triggered, this, [this, index, isFavorite]() {
        if (index.isValid()) {
            slotChangeFavorite(index, isFavorite);
        }
    });
    menu.addAction(favoriteAction);

    auto hideChannel = new QAction(QIcon::fromTheme(QStringLiteral("hide_table_row")), i18n("Hide Channel"), &menu);
    connect(hideChannel, &QAction::triggered, this, [this, index, roomType]() {
        if (index.isValid()) {
            slotHideChannel(index, roomType);
        }
    });

    if (roomType == Room::RoomType::Channel || roomType == Room::RoomType::Private) { // Not direct channel
        const QString roomId = index.data(RoomModel::RoomId).toString();
        Room *room = mCurrentRocketChatAccount->room(roomId);
        if (mCurrentRocketChatAccount->teamEnabled()) {
            if (room) {
                const bool mainTeam = index.data(RoomModel::RoomTeamIsMain).toBool();
                if (!mainTeam) {
                    const QString mainTeamId = index.data(RoomModel::RoomTeamId).toString();
                    if (mainTeamId.isEmpty() && room->hasPermission(QStringLiteral("convert-team"))) {
                        menu.addSeparator();
                        auto convertToTeam = new QAction(i18n("Convert to Team"), &menu);
                        connect(convertToTeam, &QAction::triggered, this, [this, index, roomType]() {
                            if (index.isValid()) {
                                slotConvertToTeam(index, roomType);
                            }
                        });
                        menu.addAction(convertToTeam);
                    }
                } else {
                    if (room->hasPermission(QStringLiteral("convert-team"))) {
                        menu.addSeparator();
                        auto convertToChanne = new QAction(i18n("Convert to Channel"), &menu);
                        connect(convertToChanne, &QAction::triggered, this, [this, index]() {
                            if (index.isValid()) {
                                slotConvertToChannel(index);
                            }
                        });
                        menu.addAction(convertToChanne);
                    }
                }
                const QString mainTeamId = index.data(RoomModel::RoomTeamId).toString();
                if (mainTeamId.isEmpty() && !mainTeam && room->hasPermission(QStringLiteral("add-team-channel"))) {
                    menu.addSeparator();
                    auto moveToTeam = new QAction(i18n("Move to Team"), &menu);
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
            auto configureNotificationChannel =
                new QAction(QIcon::fromTheme(QStringLiteral("preferences-desktop-notification")), i18n("Configure Notification..."), &menu);
            connect(configureNotificationChannel, &QAction::triggered, this, [this, room]() {
                slotConfigureNotification(room);
            });
            menu.addAction(configureNotificationChannel);
        }
        menu.addSeparator();
        auto quitChannel = new QAction(QIcon::fromTheme(QStringLiteral("dialog-close")), i18n("Quit Channel"), &menu);
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
        const QString teamId = dlg->teamId();
        if (!teamId.isEmpty()) {
            auto job = new RocketChatRestApi::TeamAddRoomsJob(this);
            job->setTeamId(teamId);
            const QString roomId = index.data(RoomModel::RoomId).toString();
            job->setRoomIds({roomId});

            mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
            // connect(job, &RocketChatRestApi::TeamAddRoomsJob::teamAddRoomsDone, this, &ChannelListView::slotChannelConvertToTeamDone);
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start TeamAddRoomsJob job";
            }
        } else {
            KMessageBox::information(this, i18n("Any team selected."), i18n("Move To Team"));
        }
    }
    delete dlg;
}

void ChannelListView::slotConvertToChannel(const QModelIndex &index)
{
    const QString teamId = index.data(RoomModel::RoomTeamId).toString();
    auto job = new RocketChatRestApi::TeamsListRoomsJob(this);
    job->setTeamId(teamId);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::TeamsListRoomsJob::teamListRoomsDone, this, [this, teamId, index](const QJsonObject &obj) {
        const QVector<TeamRoom> teamRooms = TeamRoom::parseTeamRooms(obj);
        QStringList listRoomIdToDelete;
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
        connect(job, &RocketChatRestApi::TeamConvertToChannelJob::teamConvertToChannelDone, this, []() {
            // TODO ?
        });
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
            connect(job, &RocketChatRestApi::ChannelsConvertToTeamJob::channelConvertToTeamDone, this, &ChannelListView::slotChannelConvertToTeamDone);
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ChannelsConvertToTeamJob job";
            }
            break;
        }
        case Room::RoomType::Private: {
            auto job = new RocketChatRestApi::GroupsConvertToTeamJob(this);
            job->setRoomId(roomId);
            mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::GroupsConvertToTeamJob::groupConvertToTeamDone, this, &ChannelListView::slotGroupConvertToTeamDone);
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ChannelsConvertToTeamJob job";
            }
            break;
        }
        }
    }
}

void ChannelListView::slotChannelConvertToTeamDone(const QJsonObject &obj)
{
    Q_UNUSED(obj)
    // qDebug() << " obj "<< obj;
    // TODO
}

void ChannelListView::slotGroupConvertToTeamDone(const QJsonObject &obj)
{
    Q_UNUSED(obj)
    // qDebug() << " obj "<< obj;
    // TODO
}

void ChannelListView::slotMarkAsChannel(const QModelIndex &index, bool markAsRead)
{
    const QString roomId = index.data(RoomModel::RoomId).toString();
    if (markAsRead) {
        mCurrentRocketChatAccount->markRoomAsRead(roomId);
    } else {
        mCurrentRocketChatAccount->markRoomAsUnRead(roomId);
    }
}

void ChannelListView::channelSelected(const QModelIndex &index)
{
    if (!index.parent().isValid())
        return;

    const QString roomId = index.data(RoomModel::RoomId).toString();
    const QString roomName = index.data(RoomModel::RoomFName).toString();
    const auto roomType = index.data(RoomModel::RoomType).value<Room::RoomType>();
    Q_EMIT roomSelected(roomName, roomId, roomType);
}

void ChannelListView::slotHideChannel(const QModelIndex &index, Room::RoomType roomType)
{
    const QString roomId = index.data(RoomModel::RoomId).toString();
    mCurrentRocketChatAccount->hideRoom(roomId, roomType);
}

void ChannelListView::slotLeaveChannel(const QModelIndex &index, Room::RoomType roomType)
{
    const QString roomId = index.data(RoomModel::RoomId).toString();
    mCurrentRocketChatAccount->leaveRoom(roomId, roomType);
}

void ChannelListView::slotChangeFavorite(const QModelIndex &index, bool isFavorite)
{
    const QString roomId = index.data(RoomModel::RoomId).toString();
    mCurrentRocketChatAccount->changeFavorite(roomId, !isFavorite);
}

void ChannelListView::selectChannelRequested(const QString &channelId, const QString &messageId)
{
    if (!selectChannelByRoomIdRequested(channelId)) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Room not found:" << channelId;
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
            const auto identifier = roomId ? roomModelIndex.data(RoomModel::RoomId).toString() : roomModelIndex.data(RoomModel::RoomName).toString();
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

void ChannelListView::slotRoomRemoved(const QString &roomId)
{
    const auto currentlySelectedIndex = selectionModel()->currentIndex();
    const QString currentRoomId = currentlySelectedIndex.data(RoomModel::RoomId).toString();
    if (currentRoomId == roomId) {
        selectNextChannel();
    }
}

#include "moc_channellistview.cpp"
