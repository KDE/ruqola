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

#include "ruqola.h"
#include "ruqolamainwidget.h"
#include "channellist/channellistview.h"
#include "channellist/channellistwidget.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
#include "room/roomwidget.h"
#include "ruqolawidgets_debug.h"

#include <KConfigGroup>
#include <KSharedConfig>
#include <QHBoxLayout>
#include <QSplitter>

#include <model/roomfilterproxymodel.h>
#include <model/roommodel.h>

#include <algorithm>

namespace
{
static const char myRuqolaMainWidgetGroupName[] = "RuqolaMainWidget";
}

RuqolaMainWidget::RuqolaMainWidget(QWidget *parent)
    : QWidget(parent)
    , mSplitter(new QSplitter(this))
    , mChannelList(new ChannelListWidget(this))
    , mStackedRoomWidget(new QStackedWidget(this))
    , mRoomWidget(new RoomWidget(this))
    , mEmptyRoomWidget(new QWidget(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));

    mSplitter->setObjectName(QStringLiteral("mSplitter"));
    mSplitter->setChildrenCollapsible(false);
    mainLayout->addWidget(mSplitter);

    mChannelList->setObjectName(QStringLiteral("mChannelList"));
    mChannelList->setLayoutSpacing(mSplitter->handleWidth());
    mSplitter->addWidget(mChannelList);

    mStackedRoomWidget->setObjectName(QStringLiteral("mStackedRoomWidget"));
    mSplitter->addWidget(mStackedRoomWidget);

    mRoomWidget->setObjectName(QStringLiteral("mRoomWidget"));
    mRoomWidget->setLayoutSpacing(mSplitter->handleWidth());
    mStackedRoomWidget->addWidget(mRoomWidget);
    connect(mRoomWidget, &RoomWidget::selectChannelRequested, this, [this](const QString &channelId) {
        mChannelList->channelListView()->activateChannelById(channelId);
    });

    mEmptyRoomWidget->setObjectName(QStringLiteral("mEmptyRoomWidget"));
    mStackedRoomWidget->addWidget(mEmptyRoomWidget);

    mStackedRoomWidget->setCurrentWidget(mEmptyRoomWidget);

    connect(mChannelList, &ChannelListWidget::channelActivated, this, &RuqolaMainWidget::selectChannelRoom);

    KConfigGroup group(KSharedConfig::openConfig(), myRuqolaMainWidgetGroupName);
    mSplitter->restoreState(group.readEntry("SplitterSizes", QByteArray()));
}

RuqolaMainWidget::~RuqolaMainWidget()
{
    KConfigGroup group(KSharedConfig::openConfig(), myRuqolaMainWidgetGroupName);
    group.writeEntry("SplitterSizes", mSplitter->saveState());
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->settings()->setLastSelectedRoom(mRoomWidget->roomId());
    }
}

void RuqolaMainWidget::selectChannelRoom(const QString &acct, const QString &roomId, const QString &roomType)
{
    Ruqola::self()->setCurrentAccount(acct, false);
    mRoomWidget->setChannelSelected(roomId, roomType);
    mStackedRoomWidget->setCurrentWidget(mRoomWidget);
    Q_EMIT channelSelected();
}

Room *RuqolaMainWidget::room() const
{
    return mRoomWidget->room();
}

QString RuqolaMainWidget::roomId() const
{
    return mRoomWidget->roomId();
}

QString RuqolaMainWidget::roomType() const
{
    return mRoomWidget->roomType();
}

void RuqolaMainWidget::setCurrentRocketChatAccount(RocketChatAccount *account, bool showLastRoom)
{
    // Before switching rocketchataccount otherwise
    mRoomWidget->storeRoomSettings();
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->settings()->setLastSelectedRoom(mRoomWidget->roomId());
    }
    mCurrentRocketChatAccount = account;
    mChannelList->setCurrentRocketChatAccount(account);
    mRoomWidget->setCurrentRocketChatAccount(account);
    mStackedRoomWidget->setCurrentWidget(mEmptyRoomWidget);

    if (showLastRoom)
        mChannelList->channelListView()->activateChannelById(mCurrentRocketChatAccount->settings()->lastSelectedRoom());
}

void RuqolaMainWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    // HACK: beautify the GUI by aligning a couple of items
    auto searchRoom = mChannelList->findChild<QWidget*>(QStringLiteral("mSearchRoom"));
    if (!searchRoom) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Failed to find mSearchRoom" << searchRoom;
        return;
    }

    auto messageLine = mRoomWidget->findChild<QWidget*>(QStringLiteral("mMessageLineWidget"));
    if (!messageLine) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Failed to find mMessageLineWidget" << messageLine;
        return;
    }

    auto align = [](QWidget *left, QWidget *right)
    {
        // the widgets on the right can be much taller, but usually are
        // just a few pixels shorter than the corresponding row on the left
        // so we just want to grow the right widget to the minimum height
        // of the left widget
        const auto minHeight = std::max(left->minimumSizeHint().height(), right->minimumSizeHint().height());
        right->setMinimumHeight(minHeight);
    };
    align(searchRoom, messageLine);
}
