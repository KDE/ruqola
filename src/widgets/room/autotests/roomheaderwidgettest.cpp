/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomheaderwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "room/roomheaderlabel.h"
#include "room/roomheaderwidget.h"
#include "room/teamnamelabel.h"
#include <QLabel>
#include <QSignalSpy>
#include <QTest>
#include <QToolButton>
#include <QVBoxLayout>
#include <qtestmouse.h>
QTEST_MAIN(RoomHeaderWidgetTest)
RoomHeaderWidgetTest::RoomHeaderWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void RoomHeaderWidgetTest::shouldHaveDefaultValues()
{
    RoomHeaderWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mRoomName = w.findChild<QLabel *>(u"mRoomName"_s);
    QVERIFY(mRoomName);
    QVERIFY(mRoomName->text().isEmpty());
    QVERIFY(!mRoomName->isVisible());
    QCOMPARE(mRoomName->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto roomNameLayout = w.findChild<QHBoxLayout *>(u"roomNameLayout"_s);
    QVERIFY(roomNameLayout);
    QCOMPARE(roomNameLayout->contentsMargins(), QMargins{});

    auto mTeamName = w.findChild<TeamNameLabel *>(u"mTeamName"_s);
    QVERIFY(mTeamName);
    QVERIFY(mTeamName->text().isEmpty());
    QVERIFY(!mTeamName->isVisible());
    QCOMPARE(mTeamName->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto mRoomHeaderLabel = w.findChild<RoomHeaderLabel *>(u"mRoomHeaderLabel"_s);
    QVERIFY(mRoomHeaderLabel);

    auto mFavoriteButton = w.findChild<QToolButton *>(u"mFavoriteButton"_s);
    QVERIFY(mFavoriteButton);
    QVERIFY(mFavoriteButton->isCheckable());
    QVERIFY(mFavoriteButton->autoRaise());

    auto mEncryptedButton = w.findChild<QToolButton *>(u"mEncryptedButton"_s);
    QVERIFY(mEncryptedButton);
    QVERIFY(mEncryptedButton->isCheckable());
    QVERIFY(!mEncryptedButton->isVisible());
    QVERIFY(mEncryptedButton->autoRaise());

    auto mDiscussionBackButton = w.findChild<QToolButton *>(u"mDiscussionBackButton"_s);
    QVERIFY(mDiscussionBackButton);
    QVERIFY(!mDiscussionBackButton->isCheckable());
    QVERIFY(!mDiscussionBackButton->isVisible());
    QVERIFY(mDiscussionBackButton->autoRaise());

    auto mChannelInfoButton = w.findChild<QToolButton *>(u"mChannelInfoButton"_s);
    QVERIFY(mChannelInfoButton);
    QVERIFY(!mChannelInfoButton->isCheckable());
    QVERIFY(!mChannelInfoButton->toolTip().isEmpty());
    QVERIFY(!mChannelInfoButton->icon().isNull());
    QVERIFY(mChannelInfoButton->autoRaise());

    auto mCallButton = w.findChild<QToolButton *>(u"mCallButton"_s);
    QVERIFY(mCallButton);
    QVERIFY(!mCallButton->isCheckable());
    QVERIFY(!mCallButton->toolTip().isEmpty());
    QVERIFY(!mCallButton->icon().isNull());
    QVERIFY(mCallButton->autoRaise());

    auto mAIActionButton = w.findChild<QToolButton *>(u"mAIActionButton"_s);
    QVERIFY(mAIActionButton);
    QCOMPARE(mAIActionButton->popupMode(), QToolButton::InstantPopup);
    QVERIFY(!mAIActionButton->isCheckable());
    QVERIFY(!mAIActionButton->toolTip().isEmpty());
    QVERIFY(!mAIActionButton->icon().isNull());
    QVERIFY(mAIActionButton->autoRaise());

    auto mListOfUsersButton = w.findChild<QToolButton *>(u"mListOfUsersButton"_s);
    QVERIFY(mListOfUsersButton);
    QVERIFY(mListOfUsersButton->isCheckable());
    QVERIFY(!mListOfUsersButton->isChecked());
    QVERIFY(!mListOfUsersButton->toolTip().isEmpty());
    QVERIFY(!mListOfUsersButton->icon().isNull());
    QVERIFY(mListOfUsersButton->autoRaise());

    auto mSearchMessageButton = w.findChild<QToolButton *>(u"mSearchMessageButton"_s);
    QVERIFY(mSearchMessageButton);
    QVERIFY(!mSearchMessageButton->isCheckable());
    QVERIFY(!mSearchMessageButton->toolTip().isEmpty());
    QVERIFY(!mSearchMessageButton->icon().isNull());
    QVERIFY(mSearchMessageButton->autoRaise());

    auto mChannelAction = w.findChild<QToolButton *>(u"mChannelAction"_s);
    QVERIFY(mChannelAction);
    QCOMPARE(mChannelAction->popupMode(), QToolButton::InstantPopup);
    QVERIFY(!mChannelAction->icon().isNull());
    QVERIFY(mChannelAction->autoRaise());

    auto mTeamChannelsButton = w.findChild<QToolButton *>(u"mTeamChannelsButton"_s);
    QVERIFY(mTeamChannelsButton);
    QVERIFY(!mTeamChannelsButton->icon().isNull());
    QVERIFY(mTeamChannelsButton->autoRaise());
    QVERIFY(!mTeamChannelsButton->toolTip().isEmpty());
}

void RoomHeaderWidgetTest::shouldShowHideIcon()
{
    RoomHeaderWidget w;
    w.show();
    // QVERIFY(QTest::qWaitForWindowExposed(&w));

    auto mFavoriteButton = w.findChild<QToolButton *>(u"mFavoriteButton"_s);
    auto mDiscussionBackButton = w.findChild<QToolButton *>(u"mDiscussionBackButton"_s);

    w.setIsDiscussion(true);
    QVERIFY(!mFavoriteButton->isVisible());
    QVERIFY(mDiscussionBackButton->isVisible());

    w.setIsDiscussion(false);
    QVERIFY(mFavoriteButton->isVisible());
    QVERIFY(!mDiscussionBackButton->isVisible());
}

void RoomHeaderWidgetTest::shouldEmitSignal()
{
    RoomHeaderWidget w;
    w.show();
    // QVERIFY(QTest::qWaitForWindowExposed(&w));

    QSignalSpy favoriteSignal(&w, &RoomHeaderWidget::favoriteChanged);
    auto mFavoriteButton = w.findChild<QToolButton *>(u"mFavoriteButton"_s);
    QTest::mouseClick(mFavoriteButton, Qt::LeftButton);
    QCOMPARE(favoriteSignal.count(), 1);

    auto mSearchMessageButton = w.findChild<QToolButton *>(u"mSearchMessageButton"_s);

    QSignalSpy searchMessageSignal(&w, &RoomHeaderWidget::searchMessageRequested);
    QTest::mouseClick(mSearchMessageButton, Qt::LeftButton);
    QCOMPARE(searchMessageSignal.count(), 1);
}

#include "moc_roomheaderwidgettest.cpp"
