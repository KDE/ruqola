/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolamainwidget.h"

#include "bannerinfodialog/bannermessagewidget.h"
#include "channellist/channellistview.h"
#include "config-ruqola.h"
#include "ddpapi/ddpclient.h"
#include "model/switchchannelhistorymodel.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
#include "room/messagetextedit.h"
#include "room/roomwidget.h"
#include "ruqolawidgets_debug.h"
#include <TextAddonsWidgets/NeedUpdateVersionWidget>

#include <KConfigGroup>
#include <KSharedConfig>
#include <QEvent>
#include <QHBoxLayout>
#include <QSplitter>
#include <QStackedWidget>
#include <QStyle>
#include <QTextBlock>
#include <QTextLayout>
#include <QTimer>

#include "model/roommodel.h"

#include <algorithm>
#include <utility>

namespace
{
const char myRuqolaMainWidgetGroupName[] = "RuqolaMainWidget";
}
using namespace Qt::Literals::StringLiterals;
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
    mTopLayout->setObjectName(u"topLayout"_s);

    auto mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(u"mainlayout"_s);

    mTopLayout->addLayout(mainLayout);

    mSplitter->setObjectName(u"mSplitter"_s);
    mSplitter->setChildrenCollapsible(false);
    mainLayout->addWidget(mSplitter);

    mChannelList->setObjectName(u"mChannelList"_s);
    mChannelList->setLayoutSpacing(mSplitter->handleWidth());
    mSplitter->addWidget(mChannelList);

    mStackedRoomWidget->setObjectName(u"mStackedRoomWidget"_s);
    mSplitter->addWidget(mStackedRoomWidget);

    mRoomWidget->setObjectName(u"mRoomWidget"_s);
    mRoomWidget->setLayoutSpacing(mSplitter->handleWidth());
    mStackedRoomWidget->addWidget(mRoomWidget);
    connect(mRoomWidget, &RoomWidget::selectChannelRequested, this, [this](const QByteArray &channelId) {
        mChannelList->channelListView()->selectChannelRequested(channelId, QByteArray());
    });

    mEmptyRoomWidget->setObjectName(u"mEmptyRoomWidget"_s);
    mStackedRoomWidget->addWidget(mEmptyRoomWidget);

    mStackedRoomWidget->setCurrentWidget(mEmptyRoomWidget);

    connect(mChannelList, &ChannelListWidget::roomSelected, this, &RuqolaMainWidget::selectChannelRoom);
    connect(mChannelList, &ChannelListWidget::roomPressed, this, &RuqolaMainWidget::slotRoomPressed);
    connect(mChannelList, &ChannelListWidget::forceLineEditFocus, mRoomWidget, &RoomWidget::forceLineEditFocus);
    connect(mChannelList, &ChannelListWidget::selectMessageIdRequested, mRoomWidget, &RoomWidget::scrollToMessageId);

    const KConfigGroup group(KSharedConfig::openConfig(), QLatin1StringView(myRuqolaMainWidgetGroupName));
    mSplitter->restoreState(group.readEntry("SplitterSizes", QByteArray()));
    if (TextAddonsWidgets::NeedUpdateVersionUtils::checkVersion()) {
        const auto status = TextAddonsWidgets::NeedUpdateVersionUtils::obsoleteVersionStatus(QStringLiteral(RUQOLA_RELEASE_VERSION), QDate::currentDate());
        if (status != TextAddonsWidgets::NeedUpdateVersionUtils::ObsoleteVersion::NotObsoleteYet) {
            auto needUpdateVersionWidget = new TextAddonsWidgets::NeedUpdateVersionWidget(this);
            needUpdateVersionWidget->setObjectName(u"needUpdateVersionWidget"_s);
            mTopLayout->insertWidget(0, needUpdateVersionWidget);
            needUpdateVersionWidget->setObsoleteVersion(status);
        }
    }
}

RuqolaMainWidget::~RuqolaMainWidget()
{
    KConfigGroup group(KSharedConfig::openConfig(), QLatin1StringView(myRuqolaMainWidgetGroupName));
    group.writeEntry("SplitterSizes", mSplitter->saveState());
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->settings()->setLastSelectedRoom(mRoomWidget->roomId());
    }
}

void RuqolaMainWidget::createBannerMessageWidget()
{
    mBannerMessageWidget = new BannerMessageWidget(this);
    mBannerMessageWidget->setObjectName(u"mBannerMessageWidget"_s);
    mTopLayout->insertWidget(0, mBannerMessageWidget);
    connect(mBannerMessageWidget, &BannerMessageWidget::infoWasRead, this, &RuqolaMainWidget::slotMarkBannerAsRead);
}

void RuqolaMainWidget::slotRoomPressed(const QByteArray &roomId)
{
    if (mRoomWidget->roomId() == roomId) {
        // force select lineedit
        mRoomWidget->forceLineEditFocus();
    }
}

void RuqolaMainWidget::selectChannelRoom(const ChannelListView::ChannelSelectedInfo &roomInfo)
{
    if (mRoomWidget->roomId() == roomInfo.roomId) {
        // During reconnect
        mRoomWidget->setChannelSelected(roomInfo.roomId, roomInfo.roomType);
        return;
    }
    mCurrentRocketChatAccount->setLastSelectedRoom(roomInfo.roomId);
    mRoomWidget->setChannelSelected(roomInfo.roomId, roomInfo.roomType);
    mStackedRoomWidget->setCurrentWidget(mRoomWidget);
    const SwitchChannelHistoryModel::SwitchChannelInfo info{roomInfo.roomName, roomInfo.roomId, roomInfo.avatarInfo};
    mCurrentRocketChatAccount->switchChannelHistoryModel()->addHistory(info);
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

QByteArray RuqolaMainWidget::roomId() const
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
    if (mCurrentRocketChatAccount) {
        connect(mCurrentRocketChatAccount, &RocketChatAccount::bannerInfoChanged, this, &RuqolaMainWidget::updateBannerInfo);
        mChannelList->setCurrentRocketChatAccount(account);
        mRoomWidget->setCurrentRocketChatAccount(account);
        mStackedRoomWidget->setCurrentWidget(mEmptyRoomWidget);

        // This is for switching between already-loaded accounts
        // On startup it's too early
        if (previousRocketChatAccount) {
            mChannelList->channelListView()->selectChannelRequested(mCurrentRocketChatAccount->settings()->lastSelectedRoom(), QByteArray());
        }
        updateBannerInfo();
    }
}

void RuqolaMainWidget::slotMarkBannerAsRead(const QByteArray &identifier)
{
    mCurrentRocketChatAccount->ddp()->bannerDismiss(identifier);
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

    if (!mSearchRoom) {
        mSearchRoom = mChannelList->findChild<QWidget *>(u"mSearchRoom"_s);
    }
    if (!mSearchRoom) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Failed to find mSearchRoom" << mSearchRoom;
        return;
    }

    if (!mMessageTextEdit) {
        mMessageTextEdit = mRoomWidget->findChild<MessageTextEdit *>(u"mMessageTextEdit"_s);
        if (mMessageTextEdit) {
            mMessageTextEdit->installEventFilter(this);
        }
    }
    if (!mMessageTextEdit) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Failed to find mMessageTextEdit" << mMessageTextEdit;
        return;
    }

    alignMessageInputs();
}

bool RuqolaMainWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == mMessageTextEdit && (event->type() == QEvent::Resize || event->type() == QEvent::FontChange || event->type() == QEvent::StyleChange)) {
        // Font and style changes can update the size hint before the layout has
        // assigned the editor its final height.
        QTimer::singleShot(0, this, &RuqolaMainWidget::alignMessageInputs);
    }
    return QWidget::eventFilter(watched, event);
}

void RuqolaMainWidget::alignMessageInputs()
{
    if (!mSearchRoom || !mMessageTextEdit || mMessageTextEdit->height() <= 0) {
        return;
    }

    int visualLineCount = 0;
    for (QTextBlock block = mMessageTextEdit->document()->begin(); block.isValid(); block = block.next()) {
        const QTextLayout *layout = block.layout();
        visualLineCount += layout ? std::max(1, layout->lineCount()) : 1;
        if (visualLineCount > 1) {
            return;
        }
    }

    if (visualLineCount == 1) {
        // Match the actual one-line editor height. Multiline heights are ignored.
        const bool breezeStyle = mSearchRoom->style()->name().compare(u"breeze"_s, Qt::CaseInsensitive) == 0;
        mSearchRoom->setFixedHeight(mMessageTextEdit->height() + (breezeStyle ? 1 : 0));
    }
}

#include "moc_ruqolamainwidget.cpp"
