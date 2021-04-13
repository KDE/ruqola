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

#include "roomheaderwidgettest.h"
#include "room/roomheaderwidget.h"
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
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    auto mRoomName = w.findChild<QLabel *>(QStringLiteral("mRoomName"));
    QVERIFY(mRoomName);
    QVERIFY(mRoomName->text().isEmpty());
    QVERIFY(!mRoomName->isVisible());
    QCOMPARE(mRoomName->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto mTopic = w.findChild<QLabel *>(QStringLiteral("mTopic"));
    QVERIFY(mTopic);
    QVERIFY(mTopic->text().isEmpty());
    QVERIFY(!mTopic->isVisible());
    QVERIFY(mTopic->wordWrap());
    QCOMPARE(mTopic->textFormat(), Qt::RichText);
    QCOMPARE(mTopic->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto mAnnouncement = w.findChild<QLabel *>(QStringLiteral("mAnnouncement"));
    QVERIFY(mAnnouncement);
    QVERIFY(mAnnouncement->text().isEmpty());
    QVERIFY(!mAnnouncement->isVisible());
    QVERIFY(mAnnouncement->wordWrap());
    QCOMPARE(mAnnouncement->textFormat(), Qt::RichText);
    QCOMPARE(mAnnouncement->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto mDescription = w.findChild<QLabel *>(QStringLiteral("mDescription"));
    QVERIFY(mDescription);
    QVERIFY(mDescription->text().isEmpty());
    QVERIFY(!mDescription->isVisible());
    QVERIFY(mDescription->wordWrap());
    QCOMPARE(mDescription->textFormat(), Qt::RichText);
    QCOMPARE(mDescription->textInteractionFlags(), Qt::TextBrowserInteraction);

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
    QCOMPARE(mTeamChannelsButton->popupMode(), QToolButton::InstantPopup);
    QVERIFY(!mTeamChannelsButton->icon().isNull());
    QVERIFY(mTeamChannelsButton->autoRaise());
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
