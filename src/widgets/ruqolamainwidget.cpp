/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolamainwidget.h"
#include "bannerinfodialog/bannermessagewidget.h"
#include "channellist/channellistview.h"
#include "channellist/channellistwidget.h"
#include "model/switchchannelhistorymodel.h"
#include "needupdateversion/needupdateversionwidget.h"
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
    , mTopLayout(new QVBoxLayout(this))
{
    mTopLayout->setContentsMargins({});
    mTopLayout->setObjectName(QStringLiteral("topLayout"));

    auto mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainlayout"));

    mTopLayout->addLayout(mainLayout);

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
        mChannelList->channelListView()->selectChannelRequested(channelId, QString());
    });

    mEmptyRoomWidget->setObjectName(QStringLiteral("mEmptyRoomWidget"));
    mStackedRoomWidget->addWidget(mEmptyRoomWidget);

    mStackedRoomWidget->setCurrentWidget(mEmptyRoomWidget);

    connect(mChannelList, &ChannelListWidget::roomSelected, this, &RuqolaMainWidget::selectChannelRoom);
    connect(mChannelList, &ChannelListWidget::roomPressed, this, &RuqolaMainWidget::slotRoomPressed);
    connect(mChannelList, &ChannelListWidget::selectMessageIdRequested, mRoomWidget, &RoomWidget::scrollToMessageId);

    KConfigGroup group(KSharedConfig::openConfig(), QLatin1String(myRuqolaMainWidgetGroupName));
    mSplitter->restoreState(group.readEntry("SplitterSizes", QByteArray()));
}

RuqolaMainWidget::~RuqolaMainWidget()
{
    KConfigGroup group(KSharedConfig::openConfig(), QLatin1String(myRuqolaMainWidgetGroupName));
    group.writeEntry("SplitterSizes", mSplitter->saveState());
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->settings()->setLastSelectedRoom(mRoomWidget->roomId());
    }
}

void RuqolaMainWidget::createBannerMessageWidget()
{
    mBannerMessageWidget = new BannerMessageWidget(this);
    mBannerMessageWidget->setObjectName(QStringLiteral("mBannerMessageWidget"));
    mTopLayout->insertWidget(0, mBannerMessageWidget);
    connect(mBannerMessageWidget, &BannerMessageWidget::infoWasRead, this, &RuqolaMainWidget::slotMarkBannerAsRead);
}

void RuqolaMainWidget::createNeedUpdateVersionWidget()
{
    // FIXME: use it
    mNeedUpdateVersionWidget = new NeedUpdateVersionWidget(this);
    mNeedUpdateVersionWidget->setObjectName(QStringLiteral("mNeedUpdateVersionWidget"));
    mTopLayout->insertWidget(0, mNeedUpdateVersionWidget);
}

void RuqolaMainWidget::slotRoomPressed(const QString &roomId)
{
    if (mRoomWidget->roomId() == roomId) {
        // force select lineedit
        mRoomWidget->forceLineEditFocus();
    }
}

void RuqolaMainWidget::selectChannelRoom(const QString &roomName, const QString &roomId, Room::RoomType roomType)
{
    if (mRoomWidget->roomId() == roomId) {
        return;
    }
    mCurrentRocketChatAccount->settings()->setLastSelectedRoom(roomId);
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
    const auto previousRocketChatAccount = mCurrentRocketChatAccount;
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->settings()->setLastSelectedRoom(mRoomWidget->roomId());
        disconnect(mCurrentRocketChatAccount, &RocketChatAccount::bannerInfoChanged, this, &RuqolaMainWidget::updateBannerInfo);
    }
    mCurrentRocketChatAccount = account;
    connect(mCurrentRocketChatAccount, &RocketChatAccount::bannerInfoChanged, this, &RuqolaMainWidget::updateBannerInfo);
    mChannelList->setCurrentRocketChatAccount(account);
    mRoomWidget->setCurrentRocketChatAccount(account);
    mStackedRoomWidget->setCurrentWidget(mEmptyRoomWidget);

    // This is for switching between already-loaded accounts
    // On startup it's too early
    if (previousRocketChatAccount) {
        mChannelList->channelListView()->selectChannelRequested(mCurrentRocketChatAccount->settings()->lastSelectedRoom(), QString());
    }

    updateBannerInfo();
}

void RuqolaMainWidget::slotMarkBannerAsRead(const QString &identifier)
{
#if 1
    mCurrentRocketChatAccount->ddp()->bannerDismiss(identifier);
    // qDebug() << " identifier " << identifier;
#else
    auto job = new RocketChatRestApi::BannersDismissJob(this);
    job->setBannerId(identifier);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::BannersDismissJob::dimissBannerDone, this, &RuqolaMainWidget::slotBannerDismissDone);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start BannersDismissJob job";
    }
#endif
}

void RuqolaMainWidget::slotBannerDismissDone()
{
    qDebug() << "void RuqolaMainWidget::slotBannerDismissDone()";
}

void RuqolaMainWidget::updateBannerInfo()
{
    if (!mBannerMessageWidget) {
        createBannerMessageWidget();
    }
    const auto bannerUnreadInformations = mCurrentRocketChatAccount->bannerInfos().bannerUnreadInformations();
    mBannerMessageWidget->setBannerInfos(std::move(bannerUnreadInformations));
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

#include "moc_ruqolamainwidget.cpp"
