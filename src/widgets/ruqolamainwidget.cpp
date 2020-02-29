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
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"

#include <KConfigGroup>
#include <KSharedConfig>
#include <QSplitter>
#include <QHBoxLayout>

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
    connect(mRoomWidget, &RoomWidget::selectChannelRequested, this, [this](const QString &channelId) {
        mChannelList->channelListView()->selectChannelRequested(channelId);
    });

    mEmptyRoomWidget = new QWidget(this);
    mEmptyRoomWidget->setObjectName(QStringLiteral("mEmptyRoomWidget"));
    mStackedRoomWidget->addWidget(mEmptyRoomWidget);

    mStackedRoomWidget->setCurrentWidget(mEmptyRoomWidget);

    connect(mChannelList, &ChannelListWidget::roomSelected, this, &RuqolaMainWidget::selectChannelRoom);

    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    mSplitter->restoreState(group.readEntry("SplitterSizes", QByteArray()));
}

RuqolaMainWidget::~RuqolaMainWidget()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("SplitterSizes", mSplitter->saveState());
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->settings()->setLastSelectedRoom(mRoomWidget->roomId());
    }
}

void RuqolaMainWidget::selectChannelRoom(const QString &roomId, const QString &roomType)
{
    mRoomWidget->setChannelSelected(roomId, roomType);
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
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->settings()->setLastSelectedRoom(mRoomWidget->roomId());
    }
    mCurrentRocketChatAccount = account;
    mChannelList->setCurrentRocketChatAccount(account);
    mRoomWidget->setCurrentRocketChatAccount(account);
    mStackedRoomWidget->setCurrentWidget(mEmptyRoomWidget);

    // This is for switching between already-loaded accounts
    // On startup it's too early
    mChannelList->channelListView()->selectChannelRequested(mCurrentRocketChatAccount->settings()->lastSelectedRoom());
}
