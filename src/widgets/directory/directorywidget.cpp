/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "directorywidget.h"
#include "connection.h"
#include "misc/directoryjob.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/directorybasefilterproxymodel.h"
#include "model/directoryroomsmodel.h"
#include "model/directoryroomsproxymodel.h"
#include "model/directoryteamsmodel.h"
#include "model/directoryteamsproxymodel.h"
#include "model/directoryusersmodel.h"
#include "model/directoryusersproxymodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QLabel>
#include <QMenu>
#include <QShowEvent>
#include <QTreeView>

DirectoryWidget::DirectoryWidget(DirectoryType type, QWidget *parent)
    : SearchTreeBaseWidget(parent)
    , mType(type)
{
    switch (mType) {
    case Room:
        mSearchLineEdit->setPlaceholderText(i18n("Search Channels"));
        mModel = new DirectoryRoomsModel(this);
        mProxyModelModel = new DirectoryRoomsProxyModel(mModel, this);
        break;
    case User:
        mSearchLineEdit->setPlaceholderText(i18n("Search Users"));
        mModel = new DirectoryUsersModel(this);
        mProxyModelModel = new DirectoryUsersProxyModel(mModel, this);
        break;
    case Team:
        mSearchLineEdit->setPlaceholderText(i18n("Search Teams"));
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

DirectoryWidget::~DirectoryWidget()
{
}

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
    if (index.isValid()) {
        switch (mType) {
        case Room: {
            QModelIndex modelIndex = mModel->index(index.row(), DirectoryRoomsModel::Identifier);
            const QString channelId = modelIndex.data().toString();
            modelIndex = mModel->index(index.row(), DirectoryRoomsModel::ChannelType);
            const QString channelType = modelIndex.data().toString();
            if (channelType == QLatin1String("p")) {
                Ruqola::self()->rocketChatAccount()->openPrivateGroup(channelId, RocketChatAccount::ChannelTypeInfo::RoomId);
            } else if (channelType == QLatin1String("c")) {
                Ruqola::self()->rocketChatAccount()->openChannel(channelId, RocketChatAccount::ChannelTypeInfo::RoomId);
            }
            break;
        }
        case User: {
            const QModelIndex modelIndex = mModel->index(index.row(), DirectoryUsersModel::UserId);
            const QString channelId = modelIndex.data().toString();
            Ruqola::self()->rocketChatAccount()->openDirectChannel(channelId);
            break;
        }
        case Team: {
            const QModelIndex modelIndex = mModel->index(index.row(), DirectoryTeamsModel::TeamIdentifier);
            const QString channelId = modelIndex.data().toString();
            Ruqola::self()->rocketChatAccount()->openChannel(channelId, RocketChatAccount::ChannelTypeInfo::RoomId);
            break;
        }
        case Unknown:
            break;
        }
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
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
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

    rcAccount->restApi()->initializeRestApiJob(job);
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
