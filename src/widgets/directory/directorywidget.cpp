/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorywidget.h"
#include "connection.h"
#include "misc/directoryjob.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/directoryroomsmodel.h"
#include "model/directoryroomsproxymodel.h"
#include "model/directoryteamsmodel.h"
#include "model/directoryteamsproxymodel.h"
#include "model/directoryusersmodel.h"
#include "model/directoryusersproxymodel.h"
#include "model/searchtreebasefilterproxymodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QLabel>
#include <QMenu>
#include <QShowEvent>
#include <QTreeView>

DirectoryWidget::DirectoryWidget(RocketChatAccount *account, DirectoryType type, QWidget *parent)
    : SearchTreeBaseWidget(account, parent)
    , mType(type)
{
    switch (mType) {
    case Room:
        mSearchLineEdit->setPlaceholderText(i18n("Search Channels..."));
        mModel = new DirectoryRoomsModel(this);
        mProxyModelModel = new DirectoryRoomsProxyModel(mModel, this);
        break;
    case User:
        mSearchLineEdit->setPlaceholderText(i18n("Search Users..."));
        mModel = new DirectoryUsersModel(this);
        mProxyModelModel = new DirectoryUsersProxyModel(mModel, this);
        break;
    case Team:
        mSearchLineEdit->setPlaceholderText(i18n("Search Teams..."));
        mModel = new DirectoryTeamsModel(this);
        mProxyModelModel = new DirectoryTeamsProxyModel(mModel, this);
        break;
    case Unknown:
        break;
    }
    mTreeView->setModel(mProxyModelModel);
    connect(mTreeView, &QTreeView::doubleClicked, this, [this](const QModelIndex &index) {
        if (index.isValid()) {
            const QModelIndex i = mProxyModelModel->mapToSource(index);
            slotOpen(i);
        }
    });
    hideColumns();
    connectModel();
}

DirectoryWidget::~DirectoryWidget() = default;

void DirectoryWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    const QModelIndex index = mTreeView->indexAt(pos);
    if (index.isValid()) {
        const QModelIndex i = mProxyModelModel->mapToSource(index);
        menu.addAction(i18n("Open..."), this, [this, i]() {
            slotOpen(i);
        });
    }
    menu.exec(mTreeView->viewport()->mapToGlobal(pos));
}

void DirectoryWidget::slotOpen(const QModelIndex &index)
{
    switch (mType) {
    case Room: {
        QModelIndex modelIndex = mModel->index(index.row(), DirectoryRoomsModel::Identifier);
        const QString channelId = modelIndex.data().toString();
        modelIndex = mModel->index(index.row(), DirectoryRoomsModel::ChannelType);
        const QString channelType = modelIndex.data().toString();
        if (channelType == QLatin1String("p")) {
            mRocketChatAccount->openPrivateGroup(channelId, RocketChatAccount::ChannelTypeInfo::RoomId);
        } else if (channelType == QLatin1String("c")) {
            mRocketChatAccount->openChannel(channelId, RocketChatAccount::ChannelTypeInfo::RoomId);
        }
        break;
    }
    case User: {
        const QModelIndex modelIndex = mModel->index(index.row(), DirectoryUsersModel::UserName);
        const QString userId = modelIndex.data().toString();
        // mRocketChatAccount->openDirectChannel(userId);
        //  Workaround RC 4.7.x where openDirectChannel doesn't accept userId as direct open channel REST API
        mRocketChatAccount->ddp()->openDirectChannel(userId);
        break;
    }
    case Team: {
        const QModelIndex modelIndex = mModel->index(index.row(), DirectoryTeamsModel::RoomIdentifier);
        const QString roomId = modelIndex.data().toString();
        mRocketChatAccount->openChannel(roomId, RocketChatAccount::ChannelTypeInfo::RoomId);
        break;
    }
    case Unknown:
        break;
    }
}

void DirectoryWidget::slotLoadElements(int offset, int count, const QString &searchName)
{
    RocketChatRestApi::DirectoryJob::DirectoryInfo info;
    switch (mType) {
    case Room:
        info.searchType = RocketChatRestApi::DirectoryJob::Rooms;
        break;
    case User:
        info.searchType = RocketChatRestApi::DirectoryJob::Users;
        break;
    case Team:
        info.searchType = RocketChatRestApi::DirectoryJob::Teams;
        break;
    case Unknown:
        qCWarning(RUQOLAWIDGETS_LOG) << "Invalid type it's a bug";
        return;
    }
    auto job = new RocketChatRestApi::DirectoryJob(this);
    if (!searchName.isEmpty()) {
        info.pattern = searchName;
    }
    job->setDirectoryInfo(info);
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
        connect(job, &RocketChatRestApi::DirectoryJob::directoryDone, this, &DirectoryWidget::slotLoadMoreElementDone);
    } else {
        connect(job, &RocketChatRestApi::DirectoryJob::directoryDone, this, &DirectoryWidget::slotSearchDone);
    }
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start searchRoomUser job";
    }
}

void DirectoryWidget::updateLabel()
{
    mLabelResultSearch->setText(mModel->total() == 0 ? noFoundInfo() : displayShowMessageInRoom());
}

QString DirectoryWidget::noFoundInfo() const
{
    switch (mType) {
    case Room:
        return i18n("No Room found");
    case User:
        return i18n("No User found");
    case Team:
        return i18n("No Team found");
    case Unknown:
        return {};
    }
    return {};
}

QString DirectoryWidget::displayShowMessageInRoom() const
{
    QString displayMessageStr;
    switch (mType) {
    case Room:
        displayMessageStr = i18np("%1 room (Total: %2)", "%1 rooms (Total: %2)", mModel->rowCount(), mModel->total());
        break;
    case User:
        displayMessageStr = i18np("%1 user (Total: %2)", "%1 users (Total: %2)", mModel->rowCount(), mModel->total());
        break;
    case Team:
        displayMessageStr = i18np("%1 team (Total: %2)", "%1 teams (Total: %2)", mModel->rowCount(), mModel->total());
        break;
    case Unknown:
        break;
    }
    if (!mModel->hasFullList()) {
        displayMessageStr += clickableStr();
    }
    return displayMessageStr;
}

void DirectoryWidget::fillDirectory()
{
    switch (mType) {
    case Room:
        mSearchLineEdit->setPlaceholderText(i18n("Search Rooms..."));
        break;
    case User:
        mSearchLineEdit->setPlaceholderText(i18n("Search Users..."));
        break;
    case Team:
        mSearchLineEdit->setPlaceholderText(i18n("Search Teams..."));
        break;
    case Unknown:
        qCWarning(RUQOLAWIDGETS_LOG) << "Invalid type it's a bug";
        return;
    }
}

void DirectoryWidget::showEvent(QShowEvent *event)
{
    if (!event->spontaneous() && !mWasInitialized) {
        mWasInitialized = true;
        initialize();
    }
    SearchTreeBaseWidget::showEvent(event);
}

DirectoryWidget::DirectoryType DirectoryWidget::type() const
{
    return mType;
}
