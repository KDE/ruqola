/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channellistview.h"
#include "channellistdelegate.h"
#include "connection.h"
#include "dialogs/configurenotificationdialog.h"
#include "model/roomfilterproxymodel.h"
#include "model/roomlistheadingsproxymodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
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
    : QListView(parent)
    , mChannelListDelegate(new ChannelListDelegate(this))
    , mRoomListHeadingsProxyModel(new RoomListHeadingsProxyModel(this))
{
    mChannelListDelegate->setObjectName(QStringLiteral("mChannelListDelegate"));
    setItemDelegate(mChannelListDelegate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QListView::setModel(mRoomListHeadingsProxyModel);

    connect(this, &ChannelListView::pressed, this, &ChannelListView::slotClicked);
}

ChannelListView::~ChannelListView() = default;

void ChannelListView::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    if (mCurrentRocketChatAccount) {
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::roomRemoved, this, &ChannelListView::slotRoomRemoved);
    }
    mCurrentRocketChatAccount = currentRocketChatAccount;
    connect(mCurrentRocketChatAccount, &RocketChatAccount::roomRemoved, this, &ChannelListView::slotRoomRemoved);
    mChannelListDelegate->setCurrentRocketChatAccount(currentRocketChatAccount);
}

QAbstractItemModel *ChannelListView::model() const
{
    return QListView::model();
}

RoomFilterProxyModel *ChannelListView::filterModel() const
{
    return qobject_cast<RoomFilterProxyModel *>(mRoomListHeadingsProxyModel->sourceModel());
}

void ChannelListView::setFilterModel(RoomFilterProxyModel *model)
{
    mRoomListHeadingsProxyModel->setSourceModel(model);
}

void ChannelListView::slotClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        channelSelected(index);
    }
}

void ChannelListView::contextMenuEvent(QContextMenuEvent *event)
{
    const QModelIndex index = indexAt(event->pos());
    if (!index.isValid()) {
        return;
    }
    if (index.data(RoomListHeadingsProxyModel::IsHeading).toBool()) {
        return;
    }
    QMenu menu(this);

    const auto roomType = index.data(RoomModel::RoomType).value<Room::RoomType>();

    const bool isUnRead = index.data(RoomModel::RoomAlert).toBool();
    const QString actionMarkAsText = isUnRead ? i18n("Mark As Read") : i18n("Mark As Unread");
    auto markAsChannel = new QAction(actionMarkAsText, &menu);
    connect(markAsChannel, &QAction::triggered, this, [=]() {
        slotMarkAsChannel(index, isUnRead);
    });
    menu.addAction(markAsChannel);

    const bool isFavorite = index.data(RoomModel::RoomFavorite).toBool();
    const QString actionFavoriteText = isFavorite ? i18n("Unset as Favorite") : i18n("Set as Favorite");
    auto favoriteAction = new QAction(QIcon::fromTheme(QStringLiteral("favorite")), actionFavoriteText, &menu);
    connect(favoriteAction, &QAction::triggered, this, [=]() {
        slotChangeFavorite(index, isFavorite);
    });
    menu.addAction(favoriteAction);

    auto hideChannel = new QAction(QIcon::fromTheme(QStringLiteral("hide_table_row")), i18n("Hide Channel"), &menu);
    connect(hideChannel, &QAction::triggered, this, [=]() {
        slotHideChannel(index, roomType);
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
                        connect(convertToTeam, &QAction::triggered, this, [=]() {
                            slotConvertToTeam(index, roomType);
                        });
                        menu.addAction(convertToTeam);
                    }
                } else {
                    if (room->hasPermission(QStringLiteral("convert-team"))) {
                        menu.addSeparator();
                        auto convertToChanne = new QAction(i18n("Convert to Channel"), &menu);
                        connect(convertToChanne, &QAction::triggered, this, [=]() {
                            slotConvertToChannel(index);
                        });
                        menu.addAction(convertToChanne);
                    }
                }
                const QString mainTeamId = index.data(RoomModel::RoomTeamId).toString();
                if (mainTeamId.isEmpty() && !mainTeam && room->hasPermission(QStringLiteral("add-team-channel"))) {
                    menu.addSeparator();
                    auto moveToTeam = new QAction(i18n("Move to Team"), &menu);
                    connect(moveToTeam, &QAction::triggered, this, [=]() {
                        slotMoveToTeam(index);
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
            connect(configureNotificationChannel, &QAction::triggered, this, [=]() {
                slotConfigureNotification(room);
            });
            menu.addAction(configureNotificationChannel);
        }
        menu.addSeparator();
        auto quitChannel = new QAction(QIcon::fromTheme(QStringLiteral("dialog-close")), i18n("Quit Channel"), &menu);
        connect(quitChannel, &QAction::triggered, this, [=]() {
            slotLeaveChannel(index, roomType);
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

void ChannelListView::setModel(QAbstractItemModel *model)
{
    QListView::setModel(model);
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
    if (KMessageBox::Yes == KMessageBox::warningYesNo(this, i18n("Are you sure to convert it to team? It can not be undo."), i18n("Convert to Team"))) {
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
    if (!index.data(RoomListHeadingsProxyModel::IsHeading).toBool()) {
        const QString roomId = index.data(RoomModel::RoomId).toString();
        const QString roomName = index.data(RoomModel::RoomFName).toString();
        const auto roomType = index.data(RoomModel::RoomType).value<Room::RoomType>();
        Q_EMIT roomSelected(roomName, roomId, roomType);
    }
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

void ChannelListView::selectChannelRequested(const QString &channelId)
{
    if (channelId.isEmpty()) {
        return;
    }
    QAbstractItemModel *filterModel = model();
    Q_ASSERT(filterModel);
    const int nRooms = filterModel->rowCount();
    if (nRooms == 0) {
        return; // too early, next chance when accountInitialized is emitted
    }
    for (int roomIdx = 0; roomIdx < nRooms; ++roomIdx) {
        const auto roomModelIndex = filterModel->index(roomIdx, 0);
        const auto roomId = roomModelIndex.data(RoomModel::RoomId).toString();
        if (roomId == channelId) {
            channelSelected(roomModelIndex);
            selectionModel()->setCurrentIndex(filterModel->index(roomIdx, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            return;
        }
    }
    qCWarning(RUQOLAWIDGETS_LOG) << "Room not found:" << channelId;
}

bool ChannelListView::selectChannelByRoomIdOrRoomName(const QString &id, bool roomId)
{
    if (id.isEmpty()) {
        return false;
    }
    QAbstractItemModel *filterModel = model();
    for (int roomIdx = 0, nRooms = filterModel->rowCount(); roomIdx < nRooms; ++roomIdx) {
        const auto roomModelIndex = filterModel->index(roomIdx, 0);
        const auto identifier = roomId ? roomModelIndex.data(RoomModel::RoomId).toString() : roomModelIndex.data(RoomModel::RoomName).toString();
        if (identifier == id) {
            channelSelected(roomModelIndex);
            selectionModel()->setCurrentIndex(filterModel->index(roomIdx, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            return true;
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
    switchToChannel(true);
}

void ChannelListView::switchToChannel(bool switchToNextUnreadChannel)
{
    QAbstractItemModel *filterModel = model();
    Q_ASSERT(filterModel);
    const int nRooms = filterModel->rowCount();
    if (nRooms == 0) {
        return;
    }
    int roomIdx = 0;
    // if we have a selection, start searching for the next unread channel there, otherwise start at the top
    const auto currentlySelectedIndex = selectionModel()->currentIndex();
    if (currentlySelectedIndex.isValid()) {
        roomIdx = currentlySelectedIndex.row();
    }

    const int startIndex = roomIdx;
    // iterate through to the end, and then wrap around to the starting point
    while (roomIdx < nRooms || (roomIdx % nRooms < startIndex)) {
        const auto roomModelIndex = filterModel->index(roomIdx % nRooms, 0);
        bool isUnRead = true;
        if (switchToNextUnreadChannel) {
            isUnRead = roomModelIndex.data(RoomModel::RoomAlert).toBool();
        }
        if (isUnRead && roomModelIndex != currentlySelectedIndex) {
            channelSelected(roomModelIndex);
            selectionModel()->setCurrentIndex(filterModel->index(roomIdx % nRooms, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            return;
        }
        roomIdx++;
    }
}

void ChannelListView::slotRoomRemoved(const QString &roomId)
{
    const auto currentlySelectedIndex = selectionModel()->currentIndex();
    const QString currentRoomId = currentlySelectedIndex.data(RoomModel::RoomId).toString();
    if (currentRoomId == roomId) {
        switchToChannel();
    }
}
