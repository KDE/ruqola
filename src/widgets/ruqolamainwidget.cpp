/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "ruqolamainwidget.h"
#include "channellist/channellistwidget.h"
#include "room/roomwidget.h"
#include "channellist/channellistview.h"

#include <KConfigGroup>
#include <KSharedConfig>
#include <QSplitter>
#include <QVBoxLayout>

#include <model/roomfilterproxymodel.h>
#include <model/roommodel.h>

namespace {
static const char myConfigGroupName[] = "RuqolaMainWidget";
}

RuqolaMainWidget::RuqolaMainWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));

    mSplitter = new QSplitter(this);
    mSplitter->setObjectName(QStringLiteral("mSplitter"));
    mSplitter->setChildrenCollapsible(false);
    mainLayout->addWidget(mSplitter);

    mChannelList = new ChannelListWidget(this);
    mChannelList->setObjectName(QStringLiteral("mChannelList"));
    mSplitter->addWidget(mChannelList);

    mStackedRoomWidget = new QStackedWidget(this);
    mStackedRoomWidget->setObjectName(QStringLiteral("mStackedRoomWidget"));
    mSplitter->addWidget(mStackedRoomWidget);

    mRoomWidget = new RoomWidget(this);
    mRoomWidget->setObjectName(QStringLiteral("mRoomWidget"));
    mStackedRoomWidget->addWidget(mRoomWidget);
    connect(mRoomWidget, &RoomWidget::selectChannelRequested, this, &RuqolaMainWidget::slotSelectChannelRequested);

    mEmptyRoomWidget = new QWidget(this);
    mEmptyRoomWidget->setObjectName(QStringLiteral("mEmptyRoomWidget"));
    mStackedRoomWidget->addWidget(mEmptyRoomWidget);

    mStackedRoomWidget->setCurrentWidget(mEmptyRoomWidget);

    connect(mChannelList, &ChannelListWidget::channelSelected, this, [this](const QModelIndex &index) {
        selectChannelRoom(index);
    });

    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    mSplitter->restoreState(group.readEntry("SplitterSizes", QByteArray()));
    slotSelectChannelRequested(group.readEntry("SelectedRoom", QString()));
}

RuqolaMainWidget::~RuqolaMainWidget()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("SplitterSizes", mSplitter->saveState());
    const QString selectedRoom = mChannelList->currentSelectedRoom();
    if (selectedRoom.isEmpty()) {
        group.deleteEntry("SelectedRoom");
    } else {
        group.writeEntry("SelectedRoom", selectedRoom);
    }
}

void RuqolaMainWidget::selectChannelRoom(const QModelIndex &index)
{
    Q_EMIT mRoomWidget->channelSelected(index);
    mStackedRoomWidget->setCurrentWidget(mRoomWidget);
    Q_EMIT channelSelected();
}

RoomWrapper *RuqolaMainWidget::roomWrapper() const
{
    return mRoomWidget->roomWrapper();
}

QString RuqolaMainWidget::roomId() const
{
    return mRoomWidget->roomId();
}

QString RuqolaMainWidget::roomType() const
{
    return mRoomWidget->roomType();
}

void RuqolaMainWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mChannelList->setCurrentRocketChatAccount(account);
    mRoomWidget->setCurrentRocketChatAccount(account);
    mStackedRoomWidget->setCurrentWidget(mEmptyRoomWidget);
}

void RuqolaMainWidget::slotSelectChannelRequested(const QString &channelId)
{
    if (channelId.isEmpty()) {
        return;
    }
    RoomFilterProxyModel *model = mChannelList->channelListView()->model();
    for (int roomIdx = 0, nRooms = model->rowCount(); roomIdx < nRooms; ++roomIdx) {
        const auto roomModelIndex = model->index(roomIdx, 0);
        const auto roomId = roomModelIndex.data(RoomModel::RoomID).toString();
        if (roomId == channelId) {
            selectChannelRoom(roomModelIndex);
            mChannelList->channelListView()->selectionModel()->setCurrentIndex(model->index(roomIdx, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            break;
        }
    }
}
