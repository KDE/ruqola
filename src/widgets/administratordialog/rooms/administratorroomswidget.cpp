/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorroomswidget.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorroomseditdialog.h"
#include "channels/channeldeletejob.h"
#include "connection.h"
#include "directmessage/deletedmjob.h"
#include "groups/groupsdeletejob.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/adminroomsmodel.h"
#include "rocketchataccount.h"
#include "rooms/adminroomsgetroomjob.h"
#include "rooms/adminroomsjob.h"
#include "rooms/changearchivationstatejob.h"

#include "ruqolawidgets_debug.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QTreeView>
#include <QVBoxLayout>

AdministratorRoomsWidget::AdministratorRoomsWidget(RocketChatAccount *account, QWidget *parent)
    : SearchTreeBaseWidget(account, parent)
    , mSelectRoomType(new AdministratorRoomsSelectRoomTypeWidget(this))
{
    mSelectRoomType->setObjectName(u"mSelectRoomType"_s);
    connect(mSelectRoomType, &AdministratorRoomsSelectRoomTypeWidget::filterChanged, this, &AdministratorRoomsWidget::slotFilterChanged);

    mModel = new AdminRoomsModel(this);
    mModel->setObjectName(u"mAdminRoomsModel"_s);

    mProxyModelModel = new AdminRoomsFilterProxyModel(mModel, this);
    mProxyModelModel->setObjectName(u"mAdminUsersProxyModel"_s);
    mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search rooms"));
    mTreeView->setModel(mProxyModelModel);
    mSearchLayout->addWidget(mSelectRoomType);
    hideColumns();
    connectModel();
    connect(mTreeView, &QTreeView::doubleClicked, this, [this](const QModelIndex &index) {
        if (index.isValid()) {
            const QModelIndex i = mProxyModelModel->mapToSource(index);
            slotModifyRoom(i);
        }
    });
}

AdministratorRoomsWidget::~AdministratorRoomsWidget() = default;

void AdministratorRoomsWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    const QModelIndex parentIndex = mTreeView->indexAt(pos);
    if (parentIndex.isValid()) {
        const QModelIndex index = mProxyModelModel->mapToSource(parentIndex);
        menu.addAction(QIcon::fromTheme(u"document-edit"_s), i18nc("@action", "Modify…"), this, [this, index]() {
            slotModifyRoom(index);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(u"list-remove"_s), i18nc("@action", "Remove"), this, [this, index]() {
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

void AdministratorRoomsWidget::slotGetRoomSettingsModifyDone(const QJsonObject &obj, Room::RoomType roomType)
{
    AdministratorRoomsEditDialog::RoomType admRoomType{AdministratorRoomsEditDialog::RoomType::Unknown};
    RoomInfo roomInfo;
    roomInfo.parseRoomInfo(obj);
    AdministratorRoomsEditBaseWidget::RoomEditInfo info;
    info.readOnly = roomInfo.readOnly();
    info.featured = roomInfo.featured();
    info.defaultRoom = roomInfo.defaultRoom();
    info.name = roomInfo.roomName();
    if (roomType == Room::RoomType::Direct) {
        admRoomType = AdministratorRoomsEditDialog::RoomType::DirectRoom;
    } else {
        info.topic = roomInfo.topic();
        info.announcement = roomInfo.announcement();
        info.description = roomInfo.description();
        info.userOwnerName = roomInfo.ownerName();
        admRoomType = AdministratorRoomsEditDialog::RoomType::Channel;
    }
    QPointer<AdministratorRoomsEditDialog> dlg = new AdministratorRoomsEditDialog(admRoomType, this);
    dlg->setRoomEditInfo(info);
    if (dlg->exec()) {
        AdministratorRoomsEditBaseWidget::RoomEditInfo newInfo = dlg->roomEditInfo();
        const QByteArray roomIdentifier = roomInfo.identifier();
        const bool oldArchivedValue = info.archived;
        const bool newArchivedValue = newInfo.archived;
        const RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo saveInfo = convertToSaveRoomSettingsInfo(newInfo, roomType, roomIdentifier);
        auto saveRoomSettingsJob = new RocketChatRestApi::SaveRoomSettingsJob(this);
        connect(saveRoomSettingsJob,
                &RocketChatRestApi::SaveRoomSettingsJob::saveRoomSettingsDone,
                this,
                [this, oldArchivedValue, newArchivedValue](const QString &roomId) {
                    slotSaveRoomSettingsDone(roomId);
                    if (newArchivedValue != oldArchivedValue) {
                        auto changeArchivationStateJob = new RocketChatRestApi::ChangeArchivationStateJob(this);
                        mRocketChatAccount->restApi()->initializeRestApiJob(changeArchivationStateJob);
                        changeArchivationStateJob->setArchive(newArchivedValue);
                        connect(changeArchivationStateJob, &RocketChatRestApi::ChangeArchivationStateJob::changeArchivationStateDone, this, []() {

                        });
                        if (!changeArchivationStateJob->start()) {
                            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start changeArchivationStateJob";
                        }
                    }
                });
        saveRoomSettingsJob->setSaveRoomSettingsInfo(saveInfo);
        mRocketChatAccount->restApi()->initializeRestApiJob(saveRoomSettingsJob);
        if (!saveRoomSettingsJob->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start saveRoomSettingsJob";
        }
    }
    delete dlg;
}

void AdministratorRoomsWidget::slotModifyRoom(const QModelIndex &index)
{
    const QString roomId = mModel->index(index.row(), AdminRoomsModel::Identifier).data().toString();
    const QString channelType = mModel->index(index.row(), AdminRoomsModel::ChannelType).data().toString();
    const Room::RoomType roomType = Room::roomTypeFromString(channelType);
    auto getRoomSettingsJob = new RocketChatRestApi::AdminRoomsGetRoomJob(this);
    getRoomSettingsJob->setRoomId(roomId);
    connect(getRoomSettingsJob, &RocketChatRestApi::AdminRoomsGetRoomJob::adminRoomGetRoomDone, this, [this, roomType](const QJsonObject &obj) {
        slotGetRoomSettingsModifyDone(obj, roomType);
    });
    mRocketChatAccount->restApi()->initializeRestApiJob(getRoomSettingsJob);
    if (!getRoomSettingsJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start AdminRoomsGetRoomJob";
    }
}

void AdministratorRoomsWidget::slotSaveRoomSettingsDone(const QString &roomId)
{
    auto getRoomSettingsJob = new RocketChatRestApi::AdminRoomsGetRoomJob(this);
    getRoomSettingsJob->setRoomId(roomId);
    connect(getRoomSettingsJob, &RocketChatRestApi::AdminRoomsGetRoomJob::adminRoomGetRoomDone, this, &AdministratorRoomsWidget::slotGetRoomSettingsDone);
    mRocketChatAccount->restApi()->initializeRestApiJob(getRoomSettingsJob);
    if (!getRoomSettingsJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start AdminRoomsGetRoomJob";
    }
}

void AdministratorRoomsWidget::slotGetRoomSettingsDone(const QJsonObject &obj)
{
    mModel->updateElement(obj);
}

RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo
AdministratorRoomsWidget::convertToSaveRoomSettingsInfo(const AdministratorRoomsEditBaseWidget::RoomEditInfo &info,
                                                        const Room::RoomType roomType,
                                                        const QByteArray &roomIdentifier)
{
    RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo roomSettingsInfo;
    roomSettingsInfo.roomId = roomIdentifier;
    roomSettingsInfo.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::SettingChanged::Favorite;
    roomSettingsInfo.favorite = info.favorite;
    roomSettingsInfo.roomType = Room::roomFromRoomType(roomType);
    if (roomType == Room::RoomType::Direct) {
        roomSettingsInfo.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::SettingChanged::Featured;
        roomSettingsInfo.featured = info.featured;
        // mDefaultCheckBox->setChecked(newRoomEditInfo.defaultRoom);
        // TODO
    } else {
        roomSettingsInfo.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::SettingChanged::RoomName;
        roomSettingsInfo.roomName = info.name;
        roomSettingsInfo.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::SettingChanged::ReadOnly;
        roomSettingsInfo.readOnly = info.readOnly;
        roomSettingsInfo.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::SettingChanged::RoomAnnouncement;
        roomSettingsInfo.roomAnnouncement = info.announcement;
        roomSettingsInfo.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::SettingChanged::RoomDescription;
        roomSettingsInfo.roomDescription = info.description;
        roomSettingsInfo.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::SettingChanged::RoomTopic;
        roomSettingsInfo.roomTopic = info.topic;
        roomSettingsInfo.mSettingsWillBeChanged |= RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo::SettingChanged::DefaultValue;
        roomSettingsInfo.defaultValue = info.defaultRoom;
    }
    return roomSettingsInfo;
}

void AdministratorRoomsWidget::slotRemoveRoom(const QModelIndex &index)
{
    const QString roomName = mModel->index(index.row(), AdminRoomsModel::Name).data().toString();
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
                                           i18n("Do you want to remove \"%1\"?", roomName),
                                           i18nc("@title", "Remove Room"),
                                           KStandardGuiItem::remove(),
                                           KStandardGuiItem::cancel())) {
        const QString roomIdentifier = mModel->index(index.row(), AdminRoomsModel::Identifier).data().toString();
        const QString channelType = mModel->index(index.row(), AdminRoomsModel::ChannelType).data().toString();
        const Room::RoomType roomType = Room::roomTypeFromString(channelType);
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
    mModel->removeElement(identifier.toLatin1());
}

void AdministratorRoomsWidget::slotDeletechannelDone(const QString &identifier)
{
    mModel->removeElement(identifier.toLatin1());
}

void AdministratorRoomsWidget::slotDeleteDirectMessageDone(const QString &identifier)
{
    mModel->removeElement(identifier.toLatin1());
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
    map.insert(u"name"_s, RocketChatRestApi::QueryParameters::SortOrder::Ascendant);
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

#include "moc_administratorroomswidget.cpp"
