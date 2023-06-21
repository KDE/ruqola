/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomheaderwidgettest.h"
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
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mRoomName = w.findChild<QLabel *>(QStringLiteral("mRoomName"));
    QVERIFY(mRoomName);
    QVERIFY(mRoomName->text().isEmpty());
    QVERIFY(!mRoomName->isVisible());
    QCOMPARE(mRoomName->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto roomNameLayout = w.findChild<QHBoxLayout *>(QStringLiteral("roomNameLayout"));
    QVERIFY(roomNameLayout);
    QCOMPARE(roomNameLayout->contentsMargins(), QMargins{});

    auto mTeamName = w.findChild<TeamNameLabel *>(QStringLiteral("mTeamName"));
    QVERIFY(mTeamName);
    QVERIFY(mTeamName->text().isEmpty());
    QVERIFY(!mTeamName->isVisible());
    QCOMPARE(mTeamName->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto mRoomHeaderLabel = w.findChild<RoomHeaderLabel *>(QStringLiteral("mRoomHeaderLabel"));
    QVERIFY(mRoomHeaderLabel);

    auto mFavoriteButton = w.findChild<QToolButton *>(QStringLiteral("mFavoriteButton"));
    QVERIFY(mFavoriteButton);
    QVERIFY(mFavoriteButton->isCheckable());
    QVERIFY(mFavoriteButton->autoRaise());

    auto mEncryptedButton = w.findChild<QToolButton *>(QStringLiteral("mEncryptedButton"));
    QVERIFY(mEncryptedButton);
    QVERIFY(mEncryptedButton->isCheckable());
    QVERIFY(!mEncryptedButton->isVisible());
    QVERIFY(mEncryptedButton->autoRaise());

    auto mDiscussionBackButton = w.findChild<QToolButton *>(QStringLiteral("mDiscussionBackButton"));
    QVERIFY(mDiscussionBackButton);
    QVERIFY(!mDiscussionBackButton->isCheckable());
    QVERIFY(!mDiscussionBackButton->isVisible());
    QVERIFY(mDiscussionBackButton->autoRaise());

    auto mChannelInfoButton = w.findChild<QToolButton *>(QStringLiteral("mChannelInfoButton"));
    QVERIFY(mChannelInfoButton);
    QVERIFY(!mChannelInfoButton->isCheckable());
    QVERIFY(!mChannelInfoButton->toolTip().isEmpty());
    QVERIFY(!mChannelInfoButton->icon().isNull());
    QVERIFY(mChannelInfoButton->autoRaise());

    auto mCallButton = w.findChild<QToolButton *>(QStringLiteral("mCallButton"));
    QVERIFY(mCallButton);
    QVERIFY(!mCallButton->isCheckable());
    QVERIFY(!mCallButton->toolTip().isEmpty());
    QVERIFY(!mCallButton->icon().isNull());
    QVERIFY(mCallButton->autoRaise());

    auto mListOfUsersButton = w.findChild<QToolButton *>(QStringLiteral("mListOfUsersButton"));
    QVERIFY(mListOfUsersButton);
    QVERIFY(mListOfUsersButton->isCheckable());
    QVERIFY(!mListOfUsersButton->isChecked());
    QVERIFY(!mListOfUsersButton->toolTip().isEmpty());
    QVERIFY(!mListOfUsersButton->icon().isNull());
    QVERIFY(mListOfUsersButton->autoRaise());

    auto mSearchMessageButton = w.findChild<QToolButton *>(QStringLiteral("mSearchMessageButton"));
    QVERIFY(mSearchMessageButton);
    QVERIFY(!mSearchMessageButton->isCheckable());
    QVERIFY(!mSearchMessageButton->toolTip().isEmpty());
    QVERIFY(!mSearchMessageButton->icon().isNull());
    QVERIFY(mSearchMessageButton->autoRaise());

    auto mChannelAction = w.findChild<QToolButton *>(QStringLiteral("mChannelAction"));
    QVERIFY(mChannelAction);
    QCOMPARE(mChannelAction->popupMode(), QToolButton::InstantPopup);
    QVERIFY(!mChannelAction->icon().isNull());
    QVERIFY(mChannelAction->autoRaise());

    auto mTeamChannelsButton = w.findChild<QToolButton *>(QStringLiteral("mTeamChannelsButton"));
    QVERIFY(mTeamChannelsButton);
    QVERIFY(!mTeamChannelsButton->icon().isNull());
    QVERIFY(mTeamChannelsButton->autoRaise());
    QVERIFY(!mTeamChannelsButton->toolTip().isEmpty());
}

void RoomHeaderWidgetTest::shouldShowHideIcon()
{
    RoomHeaderWidget w;
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));

    auto mFavoriteButton = w.findChild<QToolButton *>(QStringLiteral("mFavoriteButton"));
    auto mDiscussionBackButton = w.findChild<QToolButton *>(QStringLiteral("mDiscussionBackButton"));

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
    QVERIFY(QTest::qWaitForWindowExposed(&w));

    QSignalSpy favoriteSignal(&w, &RoomHeaderWidget::favoriteChanged);
    auto mFavoriteButton = w.findChild<QToolButton *>(QStringLiteral("mFavoriteButton"));
    QTest::mouseClick(mFavoriteButton, Qt::LeftButton);
    QCOMPARE(favoriteSignal.count(), 1);

    auto mSearchMessageButton = w.findChild<QToolButton *>(QStringLiteral("mSearchMessageButton"));

    QSignalSpy searchMessageSignal(&w, &RoomHeaderWidget::searchMessageRequested);
    QTest::mouseClick(mSearchMessageButton, Qt::LeftButton);
    QCOMPARE(searchMessageSignal.count(), 1);
}

#include "moc_roomheaderwidgettest.cpp"
