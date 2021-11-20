/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "administratorroomswidget.h"
#include "administratorroomseditdialog.h"
#include "channels/channeldeletejob.h"
#include "connection.h"
#include "directmessage/deletedmjob.h"
#include "groups/groupsdeletejob.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/adminroomsmodel.h"
#include "rocketchataccount.h"
#include "roominfo/roomsinfo.h"
#include "rooms/adminroomsjob.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <QLabel>
#include <QMenu>
#include <QTreeView>
#include <QVBoxLayout>

AdministratorRoomsWidget::AdministratorRoomsWidget(RocketChatAccount *account, QWidget *parent)
    : SearchTreeBaseWidget(account, parent)
    , mSelectRoomType(new AdministratorRoomsSelectRoomTypeWidget(this))
{
    mSelectRoomType->setObjectName(QStringLiteral("mSelectRoomType"));
    connect(mSelectRoomType, &AdministratorRoomsSelectRoomTypeWidget::filterChanged, this, &AdministratorRoomsWidget::slotFilterChanged);

    mModel = new AdminRoomsModel(this);
    mModel->setObjectName(QStringLiteral("mAdminRoomsModel"));

    mProxyModelModel = new AdminRoomsFilterProxyModel(mModel, this);
    mProxyModelModel->setObjectName(QStringLiteral("mAdminUsersProxyModel"));
    mSearchLineEdit->setPlaceholderText(i18n("Search Users"));
    mTreeView->setModel(mProxyModelModel);
    mSearchLayout->addWidget(mSelectRoomType);
    hideColumns();
    connectModel();
}

AdministratorRoomsWidget::~AdministratorRoomsWidget()
{
}

void AdministratorRoomsWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    const QModelIndex parentIndex = mTreeView->indexAt(pos);
    if (parentIndex.isValid()) {
        const QModelIndex index = mProxyModelModel->mapToSource(parentIndex);
        menu.addAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Modify..."), this, [this, index]() {
            slotModifyRoom(index);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, index]() {
            slotRemoveRoom(index);
        });
    }
    menu.exec(mTreeView->viewport()->mapToGlobal(pos));
}

void AdministratorRoomsWidget::updateLabel()
{
    mLabelResultSearch->setText(mModel->total() == 0 ? i18n("No room found") : displayShowMessageInRoom());
}

QString AdministratorRoomsWidget::displayShowMessageInRoom() const
{
    QString displayMessageStr = i18np("%1 room (Total: %2)", "%1 rooms (Total: %2)", mModel->rowCount(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += clickableStr();
    }
    return displayMessageStr;
}

void AdministratorRoomsWidget::slotModifyRoom(const QModelIndex &index)
{
    qWarning() << "modify room NOT IMPLEMENTED";
    // TODO
}

void AdministratorRoomsWidget::slotRemoveRoom(const QModelIndex &index)
{
    const QString roomName = mModel->index(index.row(), AdminRoomsModel::Name).data().toString();
    if (KMessageBox::Yes
        == KMessageBox::questionYesNo(this,
                                      i18n("Do you want to remove \"%1\"?", roomName),
                                      i18nc("@title", "Remove Room"),
                                      KStandardGuiItem::remove(),
                                      KStandardGuiItem::cancel())) {
        const QString roomIdentifier = mModel->index(index.row(), AdminRoomsModel::Identifier).data().toString();
        const QString channelType = mModel->index(index.row(), AdminRoomsModel::ChannelType).data().toString();
        Room::RoomType roomType = Room::roomTypeFromString(channelType);
        switch (roomType) {
        case Room::RoomType::Private: {
            auto job = new RocketChatRestApi::GroupsDeleteJob(this);
            mRocketChatAccount->restApi()->initializeRestApiJob(job);
            RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo info;
            info.identifier = roomIdentifier;
            info.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
            job->setChannelGroupInfo(info);
            connect(job, &RocketChatRestApi::GroupsDeleteJob::deleteGroupsDone, this, &AdministratorRoomsWidget::slotDeleteGroupsDone);
            if (!job->start()) {
                qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start GroupsDeleteJob";
            }
            break;
        }
        case Room::RoomType::Channel: {
            auto job = new RocketChatRestApi::ChannelDeleteJob(this);
            mRocketChatAccount->restApi()->initializeRestApiJob(job);
            RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo info;
            info.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
            info.identifier = roomIdentifier;
            job->setChannelGroupInfo(info);
            connect(job, &RocketChatRestApi::ChannelDeleteJob::deletechannelDone, this, &AdministratorRoomsWidget::slotDeletechannelDone);
            if (!job->start()) {
                qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start ChannelDeleteJob";
            }
            break;
        }
        case Room::RoomType::Direct: {
            auto job = new RocketChatRestApi::DeleteDmJob(this);
            mRocketChatAccount->restApi()->initializeRestApiJob(job);
            RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfo info;
            info.channelGroupInfoType = RocketChatRestApi::ChannelGroupBaseJob::ChannelGroupInfoType::Identifier;
            info.identifier = roomIdentifier;
            job->setChannelGroupInfo(info);
            connect(job, &RocketChatRestApi::DeleteDmJob::deleteDirectMessagesDone, this, &AdministratorRoomsWidget::slotDeleteDirectMessageDone);
            if (!job->start()) {
                qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start ChannelDeleteJob";
            }
            break;
        }
        case Room::RoomType::Unknown:
            qCWarning(RUQOLAWIDGETS_LOG) << " unsupported delete for type " << channelType;
            break;
        }
    }
}

void AdministratorRoomsWidget::slotDeleteGroupsDone(const QString &identifier)
{
    mModel->removeElement(identifier);
}

void AdministratorRoomsWidget::slotDeletechannelDone(const QString &identifier)
{
    mModel->removeElement(identifier);
}

void AdministratorRoomsWidget::slotDeleteDirectMessageDone(const QString &identifier)
{
    mModel->removeElement(identifier);
}

void AdministratorRoomsWidget::slotFilterChanged(AdminRoomsFilterProxyModel::FilterRooms filters)
{
    static_cast<AdminRoomsFilterProxyModel *>(mProxyModelModel)->setFilterRooms(filters);
}

void AdministratorRoomsWidget::slotLoadElements(int offset, int count, const QString &searchName)
{
    auto job = new RocketChatRestApi::AdminRoomsJob(this);
    RocketChatRestApi::AdminRoomsJob::AdminRoomsJobInfo info;
    info.filter = searchName;
    info.searchType = RocketChatRestApi::AdminRoomsJob::AdminRoomSearchType::All;
    job->setRoomsAdminInfo(info);

    RocketChatRestApi::QueryParameters parameters;
    QMap<QString, RocketChatRestApi::QueryParameters::SortOrder> map;
    map.insert(QStringLiteral("name"), RocketChatRestApi::QueryParameters::SortOrder::Ascendant);
    parameters.setSorting(map);
    if (offset != -1) {
        parameters.setOffset(offset);
    }
    if (count != -1) {
        parameters.setCount(count);
    }
    job->setQueryParameters(parameters);

    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    if (offset != -1) {
        connect(job, &RocketChatRestApi::AdminRoomsJob::adminRoomsDone, this, &AdministratorRoomsWidget::slotLoadMoreElementDone);
    } else {
        connect(job, &RocketChatRestApi::AdminRoomsJob::adminRoomsDone, this, &AdministratorRoomsWidget::slotSearchDone);
    }
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AdminRoomsJob job";
    }
}
