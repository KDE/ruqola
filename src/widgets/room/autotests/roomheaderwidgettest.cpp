/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

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
using namespace Qt::Literals::StringLiterals;
RoomHeaderWidgetTest::RoomHeaderWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void RoomHeaderWidgetTest::shouldHaveDefaultValues()
{
    const RoomHeaderWidget w;
    QVERIFY(w.sizePolicy().hasHeightForWidth());
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto headerLayout = w.findChild<QHBoxLayout *>(u"headerLayout"_s);
    QVERIFY(headerLayout);
    QCOMPARE(headerLayout->contentsMargins(), QMargins(0, 4, 0, 4));

    auto mRoomName = w.findChild<QLabel *>(u"mRoomName"_s);
    QVERIFY(mRoomName);
    QVERIFY(mRoomName->text().isEmpty());
    QVERIFY(!mRoomName->isVisible());
    QCOMPARE(mRoomName->textInteractionFlags(), Qt::TextBrowserInteraction);

    auto roomNameLayout = w.findChild<QHBoxLayout *>(u"roomNameLayout"_s);
    QVERIFY(roomNameLayout);
    QCOMPARE(roomNameLayout->contentsMargins().left(), 0);
    QCOMPARE(roomNameLayout->contentsMargins().right(), 0);
    QCOMPARE(roomNameLayout->contentsMargins().bottom(), 0);

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

void RoomHeaderWidgetTest::shouldCenterCollapsedHeaderItems()
{
    RoomHeaderWidget w;
    w.setRoomName(u"#yggdrasil"_s);

    auto roomIcon = w.findChild<QLabel *>(u"mRoomIcon"_s);
    QVERIFY(roomIcon);
    roomIcon->setPixmap(QPixmap(24, 24));

    w.resize(600, 100);
    w.show();
    QCoreApplication::processEvents();

    const auto roomName = w.findChild<QLabel *>(u"mRoomName"_s);
    const auto favoriteButton = w.findChild<QToolButton *>(u"mFavoriteButton"_s);
    const auto callButton = w.findChild<QToolButton *>(u"mCallButton"_s);
    const int centerY = favoriteButton->geometry().center().y();
    // Allow 2px: two independently centre-aligned widgets of different heights can differ by up to
    // that much from the integer truncation in geometry().center() (== y() + height()/2), and the
    // exact heights depend on the platform's default font (1px passes here but not on every distro).
    QVERIFY(qAbs(roomIcon->geometry().center().y() - centerY) <= 2);
    QVERIFY(qAbs(roomName->geometry().center().y() - centerY) <= 2);
    QVERIFY(qAbs(callButton->geometry().center().y() - centerY) <= 2);
}

void RoomHeaderWidgetTest::shouldAlignExpandedHeaderItemsWithFirstRow()
{
    RoomHeaderWidget w;
    w.setRoomName(u"#ruqola"_s);
    w.setRoomTopic(u"First line\nSecond line\nThird line\nFourth line\nFifth line"_s);

    auto roomIcon = w.findChild<QLabel *>(u"mRoomIcon"_s);
    QVERIFY(roomIcon);
    roomIcon->setPixmap(QPixmap(24, 24));

    const auto roomHeaderLabel = w.findChild<RoomHeaderLabel *>(u"mRoomHeaderLabel"_s);
    Q_EMIT roomHeaderLabel->linkActivated(u"showmoretext"_s);

    w.resize(600, 300);
    w.show();
    QCoreApplication::processEvents();

    const auto roomName = w.findChild<QLabel *>(u"mRoomName"_s);
    const auto favoriteButton = w.findChild<QToolButton *>(u"mFavoriteButton"_s);
    const auto callButton = w.findChild<QToolButton *>(u"mCallButton"_s);
    QVERIFY(roomHeaderLabel->height() > favoriteButton->height());

    const int firstRowCenterY = roomName->geometry().center().y();
    // See shouldCenterCollapsedHeaderItems(): 2px tolerance for integer rounding in
    // geometry().center() across platform fonts.
    QVERIFY(qAbs(favoriteButton->geometry().center().y() - firstRowCenterY) <= 2);
    QVERIFY(qAbs(roomIcon->geometry().center().y() - firstRowCenterY) <= 2);
    QVERIFY(qAbs(callButton->geometry().center().y() - firstRowCenterY) <= 2);
}

void RoomHeaderWidgetTest::shouldGrowExpandedHeaderForNarrowWidth()
{
    QWidget container;
    auto outerLayout = new QVBoxLayout(&container);
    outerLayout->setContentsMargins({});
    outerLayout->setSpacing(0);
    auto constrainedHeader = new RoomHeaderWidget(&container);
    constrainedHeader->setRoomName(u"#ruqola"_s);
    constrainedHeader->setRoomTopic(
        u"First line with enough text to wrap at narrow widths\nSecond line with enough text to wrap at narrow widths\nThird line with enough text to "
        u"wrap at narrow widths\nFourth line with enough text to wrap at narrow widths\nFifth line with enough text to wrap at narrow widths"_s);
    const auto constrainedLabel = constrainedHeader->findChild<RoomHeaderLabel *>(u"mRoomHeaderLabel"_s);
    Q_EMIT constrainedLabel->linkActivated(u"showmoretext"_s);
    outerLayout->addWidget(constrainedHeader);
    auto body = new QWidget(&container);
    body->setMinimumHeight(20);
    body->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    outerLayout->addWidget(body);
    container.resize(400, 300);
    container.show();
    QCoreApplication::processEvents();

    const int requiredHeaderHeight = constrainedHeader->heightForWidth(constrainedHeader->width());
    container.resize(400, requiredHeaderHeight + body->minimumHeight());
    QCoreApplication::processEvents();

    QCOMPARE(constrainedHeader->geometry().top(), 0);
    QVERIFY(constrainedHeader->height() >= requiredHeaderHeight);
    QVERIFY(constrainedLabel->height() >= constrainedLabel->heightForWidth(constrainedLabel->width()));
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

    const QSignalSpy favoriteSignal(&w, &RoomHeaderWidget::favoriteChanged);
    auto mFavoriteButton = w.findChild<QToolButton *>(u"mFavoriteButton"_s);
    QTest::mouseClick(mFavoriteButton, Qt::LeftButton);
    QCOMPARE(favoriteSignal.count(), 1);

    auto mSearchMessageButton = w.findChild<QToolButton *>(u"mSearchMessageButton"_s);

    const QSignalSpy searchMessageSignal(&w, &RoomHeaderWidget::searchMessageRequested);
    QTest::mouseClick(mSearchMessageButton, Qt::LeftButton);
    QCOMPARE(searchMessageSignal.count(), 1);
}

#include "moc_roomheaderwidgettest.cpp"
