/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolamainwidget.h"
#include "bannerinfodialog/bannermessagewidget.h"
#include "channellist/channellistview.h"
#include "channellist/channellistwidget.h"
#include "model/switchchannelhistorymodel.h"
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
    , mBannerMessageWidget(new BannerMessageWidget(this))
{
    auto topLayout = new QVBoxLayout(this);
    topLayout->setContentsMargins({});
    topLayout->setObjectName(QStringLiteral("topLayout"));

    auto mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainlayout"));

    mBannerMessageWidget->setObjectName(QStringLiteral("mBannerMessageWidget"));
    topLayout->addWidget(mBannerMessageWidget);
    topLayout->addLayout(mainLayout);

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
        mChannelList->channelListView()->selectChannelRequested(channelId);
    });

    mEmptyRoomWidget->setObjectName(QStringLiteral("mEmptyRoomWidget"));
    mStackedRoomWidget->addWidget(mEmptyRoomWidget);

    mStackedRoomWidget->setCurrentWidget(mEmptyRoomWidget);

    connect(mChannelList, &ChannelListWidget::roomSelected, this, &RuqolaMainWidget::selectChannelRoom);
    connect(mChannelList, &ChannelListWidget::selectMessageIdRequested, mRoomWidget, &RoomWidget::scrollToMessageId);

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

void RuqolaMainWidget::selectChannelRoom(const QString &roomName, const QString &roomId, Room::RoomType roomType)
{
    mRoomWidget->setChannelSelected(roomId, roomType);
    mStackedRoomWidget->setCurrentWidget(mRoomWidget);
    mCurrentRocketChatAccount->switchChannelHistoryModel()->addHistory(roomName, roomId);
    Q_EMIT channelSelected();
}

void RuqolaMainWidget::selectNextUnreadChannel()
{
    if (mChannelList && mChannelList->channelListView()) {
        mChannelList->channelListView()->selectNextUnreadChannel();
    }
}

Room *RuqolaMainWidget::room() const
{
    return mRoomWidget->room();
}

QString RuqolaMainWidget::roomId() const
{
    return mRoomWidget->roomId();
}

Room::RoomType RuqolaMainWidget::roomType() const
{
    return mRoomWidget->roomType();
}

void RuqolaMainWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
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

    // This is for switching between already-loaded accounts
    // On startup it's too early
    mChannelList->channelListView()->selectChannelRequested(mCurrentRocketChatAccount->settings()->lastSelectedRoom());
}

void RuqolaMainWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    // HACK: beautify the GUI by aligning a couple of items
    auto searchRoom = mChannelList->findChild<QWidget *>(QStringLiteral("mSearchRoom"));
    if (!searchRoom) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Failed to find mSearchRoom" << searchRoom;
        return;
    }

    auto messageLine = mRoomWidget->findChild<QWidget *>(QStringLiteral("mMessageLineWidget"));
    if (!messageLine) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Failed to find mMessageLineWidget" << messageLine;
        return;
    }

    auto align = [](QWidget *left, QWidget *right) {
        // the widgets on the right can be much taller, but usually are
        // just a few pixels shorter than the corresponding row on the left
        // so we just want to grow the right widget to the minimum height
        // of the left widget
        const auto minHeight = std::max(left->minimumSizeHint().height(), right->minimumSizeHint().height());
        right->setMinimumHeight(minHeight);
    };
    align(searchRoom, messageLine);
}
