/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "model/roommodel.h"
#include "rocketchataccount.h"
#include "room/commandpreviewwidget.h"
#include "room/messagelinewidget.h"
#include "room/messagelistview.h"
#include "room/readonlylineeditwidget.h"
#include "room/roomcounterinfowidget.h"
#include "room/roomheaderwidget.h"
#include "room/roomquotemessagewidget.h"
#include "room/roomreplythreadwidget.h"
#include "room/roomwidget.h"
#include "room/uploadfileprogressstatuslistwidget.h"
#include "room/usersinroomflowwidget.h"

#include "ruqola.h"
#include "testdata.h"

#include <QStackedWidget>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(RoomWidgetTest)
RoomWidgetTest::RoomWidgetTest(QObject *parent)
    : QObject(parent)
{
    initTestAccount();
    QStandardPaths::setTestModeEnabled(true);
}

void RoomWidgetTest::shouldHaveDefaultValues()
{
    RoomWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mRoomHeaderWidget = w.findChild<RoomHeaderWidget *>(u"mRoomHeaderWidget"_s);
    QVERIFY(mRoomHeaderWidget);

    auto mUsersInRoomFlowWidget = w.findChild<UsersInRoomFlowWidget *>(u"mUsersInRoomFlowWidget"_s);
    QVERIFY(mUsersInRoomFlowWidget);

    auto mMessageListView = w.findChild<MessageListView *>(u"mMessageListView"_s);
    QVERIFY(mMessageListView);

    auto mStackedWidget = w.findChild<QStackedWidget *>(u"mStackedWidget"_s);
    QVERIFY(mStackedWidget);
    QCOMPARE(mStackedWidget->count(), 2);

    auto mMessageLineWidget = w.findChild<MessageLineWidget *>(u"mMessageLineWidget"_s);
    QVERIFY(mMessageLineWidget);

    auto mReadOnlyLineEditWidget = w.findChild<ReadOnlyLineEditWidget *>(u"mReadOnlyLineEditWidget"_s);
    QVERIFY(mReadOnlyLineEditWidget);
    QCOMPARE(mStackedWidget->currentWidget(), mMessageLineWidget);

    QVERIFY(w.roomId().isEmpty());

    auto mRoomCounterInfoWidget = w.findChild<RoomCounterInfoWidget *>(u"mRoomCounterInfoWidget"_s);
    QVERIFY(mRoomCounterInfoWidget);

#if 0 // Load on demand
    auto mRoomReconnectInfoWidget = w.findChild<ReconnectInfoWidget *>(u"mRoomReconnectInfoWidget"_s);
    QVERIFY(mRoomReconnectInfoWidget);
#endif
    auto mRoomReplyThreadWidget = w.findChild<RoomReplyThreadWidget *>(u"mRoomReplyThreadWidget"_s);
    QVERIFY(mRoomReplyThreadWidget);
    QVERIFY(!mRoomReplyThreadWidget->isVisible());

    auto mRoomQuoteMessageWidget = w.findChild<RoomQuoteMessageWidget *>(u"mRoomQuoteMessageWidget"_s);
    QVERIFY(mRoomQuoteMessageWidget);
    QVERIFY(!mRoomQuoteMessageWidget->isVisible());

    auto mCommandPreviewWidget = w.findChild<CommandPreviewWidget *>(u"mCommandPreviewWidget"_s);
    QVERIFY(mCommandPreviewWidget);
    QVERIFY(!mCommandPreviewWidget->isVisible());

    auto mUploadFileProgressStatusListWidget = w.findChild<UploadFileProgressStatusListWidget *>(u"mUploadFileProgressStatusListWidget"_s);
    QVERIFY(mUploadFileProgressStatusListWidget);
    QVERIFY(!mUploadFileProgressStatusListWidget->isVisible());

#if 0 // Load on demand
    auto mOtrWidget = w.findChild<OtrWidget *>(u"mOtrWidget"_s);
    QVERIFY(mOtrWidget);

    auto mOffLineWidget = w.findChild<OffLineWidget *>(u"mOffLineWidget"_s);
    QVERIFY(mOffLineWidget);

    auto mPluginTextMessageWidget = w.findChild<PluginTextMessageWidget *>(u"mPluginTextMessageWidget"_s);
    QVERIFY(mPluginTextMessageWidget);
#endif
}

static Room *createRoom(const QByteArray &roomId, const QString &roomName)
{
    RocketChatAccount *rcAccount = Ruqola::self()->rocketChatAccount();
    Room *r = new Room(rcAccount);
    r->setRoomId(roomId);
    r->setName(roomName);
    const Room::RoomType roomType = Room::RoomType::Channel; // should be an enum...
    r->setChannelType(roomType);
    r->setArchived(true); // workaround to skip RestApiConnection::membersInRoom
    return r;
}

void RoomWidgetTest::shouldStorePendingTextPerRoom()
{
    // GIVEN two rooms
    RoomWidget w;
    RocketChatAccount *rcAccount = Ruqola::self()->rocketChatAccount();
    w.setCurrentRocketChatAccount(rcAccount);
    const QByteArray roomId1("roomId1");
    Room *room1 = createRoom(roomId1, u"roomName1"_s);
    QVERIFY(rcAccount->roomModel()->addRoom(room1));
    const QByteArray roomId2("roomId2");
    Room *room2 = createRoom(roomId2, u"roomName2"_s);
    QVERIFY(rcAccount->roomModel()->addRoom(room2));

    // Ensure switching between rooms works
    w.setChannelSelected(room1->roomId(), room1->channelType());
    QCOMPARE(w.roomId(), roomId1);
    w.setChannelSelected(room2->roomId(), room2->channelType());
    QCOMPARE(w.roomId(), roomId2);

    // WHEN typing text and switching rooms
    auto mMessageLineWidget = w.findChild<MessageLineWidget *>(u"mMessageLineWidget"_s);
    QVERIFY(mMessageLineWidget);
    mMessageLineWidget->setText(u"Text for room 2"_s);

    w.setChannelSelected(room1->roomId(), room1->channelType());
    // THEN the text should be empty
    QCOMPARE(mMessageLineWidget->text(), QString());

    // WHEN typing some text and switching back
    mMessageLineWidget->setText(u"Text for room 1"_s);

    // THEN the orig text should appear again
    w.setChannelSelected(room2->roomId(), room2->channelType());
    QCOMPARE(mMessageLineWidget->text(), u"Text for room 2"_s);
    mMessageLineWidget->setText(QString());

    // WHEN switching again
    mMessageLineWidget->setText(u"Text for room 1"_s);

    // THEN the other text should appear again
    w.setChannelSelected(room2->roomId(), room2->channelType());
    QCOMPARE(mMessageLineWidget->text(), u"Text for room 1"_s);
}

void RoomWidgetTest::shouldShowNoticeWhenReplyingToThread()
{
    QEventLoop loop;
    RoomWidget w;

    auto mRoomReplyThreadWidget = w.findChild<RoomReplyThreadWidget *>(u"mRoomReplyThreadWidget"_s);
    QVERIFY(!mRoomReplyThreadWidget->isVisible());

    auto mMessageLineWidget = w.findChild<MessageLineWidget *>(u"mMessageLineWidget"_s);
    QVERIFY(mMessageLineWidget);

    mMessageLineWidget->setThreadMessageId("placeholder"_ba);
    loop.processEvents();
    QVERIFY(!mRoomReplyThreadWidget->isHidden());

    mMessageLineWidget->setThreadMessageId({});
    loop.processEvents();
    QVERIFY(mRoomReplyThreadWidget->isHidden());
}

#include "moc_roomwidgettest.cpp"
