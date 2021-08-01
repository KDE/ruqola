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

#include "roomwidgettest.h"
#include "model/roommodel.h"
#include "rocketchataccount.h"
#include "room/messagelinewidget.h"
#include "room/messagelistview.h"
#include "room/readonlylineeditwidget.h"
#include "room/roomcounterinfowidget.h"
#include "room/roomheaderwidget.h"
#include "room/roomquotemessagewidget.h"
#include "room/roomreplythreadwidget.h"
#include "room/roomwidget.h"
#include "room/uploadfileprogressstatuswidget.h"
#include "room/usersinroomflowwidget.h"

#include <ruqola.h>

#include <QStackedWidget>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(RoomWidgetTest)
RoomWidgetTest::RoomWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void RoomWidgetTest::shouldHaveDefaultValues()
{
    RoomWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto mRoomHeaderWidget = w.findChild<RoomHeaderWidget *>(QStringLiteral("mRoomHeaderWidget"));
    QVERIFY(mRoomHeaderWidget);

    auto mUsersInRoomFlowWidget = w.findChild<UsersInRoomFlowWidget *>(QStringLiteral("mUsersInRoomFlowWidget"));
    QVERIFY(mUsersInRoomFlowWidget);

    auto mMessageListView = w.findChild<MessageListView *>(QStringLiteral("mMessageListView"));
    QVERIFY(mMessageListView);

    auto mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedWidget"));
    QVERIFY(mStackedWidget);
    QCOMPARE(mStackedWidget->count(), 2);

    auto mMessageLineWidget = w.findChild<MessageLineWidget *>(QStringLiteral("mMessageLineWidget"));
    QVERIFY(mMessageLineWidget);

    auto mReadOnlyLineEditWidget = w.findChild<ReadOnlyLineEditWidget *>(QStringLiteral("mReadOnlyLineEditWidget"));
    QVERIFY(mReadOnlyLineEditWidget);
    QCOMPARE(mStackedWidget->currentWidget(), mMessageLineWidget);

    QVERIFY(w.roomId().isEmpty());

    auto mRoomCounterInfoWidget = w.findChild<RoomCounterInfoWidget *>(QStringLiteral("mRoomCounterInfoWidget"));
    QVERIFY(mRoomCounterInfoWidget);

    auto mRoomReplyThreadWidget = w.findChild<RoomReplyThreadWidget *>(QStringLiteral("mRoomReplyThreadWidget"));
    QVERIFY(mRoomReplyThreadWidget);
    QVERIFY(!mRoomReplyThreadWidget->isVisible());

    auto mRoomQuoteMessageWidget = w.findChild<RoomQuoteMessageWidget *>(QStringLiteral("mRoomQuoteMessageWidget"));
    QVERIFY(mRoomQuoteMessageWidget);
    QVERIFY(!mRoomQuoteMessageWidget->isVisible());

    auto mUploadFileProgressStatusWidget = w.findChild<UploadFileProgressStatusWidget *>(QStringLiteral("mUploadFileProgressStatusWidget"));
    QVERIFY(mUploadFileProgressStatusWidget);
    QVERIFY(!mUploadFileProgressStatusWidget->isVisible());
}

static Room *createRoom(const QString &roomId, const QString &roomName)
{
    RocketChatAccount *rcAccount = Ruqola::self()->rocketChatAccount();
    Room *r = new Room(rcAccount);
    r->setRoomId(roomId);
    r->setName(roomName);
    const Room::RoomType roomType = Room::RoomType::Channel; // should be an enum...
    r->setChannelType(roomType);
    r->setArchived(true); // workaround to skip RestApiRequest::membersInRoom
    return r;
}

void RoomWidgetTest::shouldStorePendingTextPerRoom()
{
    // GIVEN two rooms
    RoomWidget w;
    RocketChatAccount *rcAccount = Ruqola::self()->rocketChatAccount();
    w.setCurrentRocketChatAccount(rcAccount);
    const QString roomId1 = QStringLiteral("roomId1");
    Room *room1 = createRoom(roomId1, QStringLiteral("roomName1"));
    QVERIFY(rcAccount->roomModel()->addRoom(room1));
    const QString roomId2 = QStringLiteral("roomId2");
    Room *room2 = createRoom(roomId2, QStringLiteral("roomName2"));
    QVERIFY(rcAccount->roomModel()->addRoom(room2));

    // Ensure switching between rooms works
    w.setChannelSelected(room1->roomId(), room1->channelType());
    QCOMPARE(w.roomId(), roomId1);
    w.setChannelSelected(room2->roomId(), room2->channelType());
    QCOMPARE(w.roomId(), roomId2);

    // WHEN typing text and switching rooms
    auto mMessageLineWidget = w.findChild<MessageLineWidget *>(QStringLiteral("mMessageLineWidget"));
    QVERIFY(mMessageLineWidget);
    mMessageLineWidget->setText(QStringLiteral("Text for room 2"));

    w.setChannelSelected(room1->roomId(), room1->channelType());
    // THEN the text should be empty
    QCOMPARE(mMessageLineWidget->text(), QString());

    // WHEN typing some text and switching back
    mMessageLineWidget->setText(QStringLiteral("Text for room 1"));

    // THEN the orig text should appear again
    w.setChannelSelected(room2->roomId(), room2->channelType());
    QCOMPARE(mMessageLineWidget->text(), QStringLiteral("Text for room 2"));
    mMessageLineWidget->setText(QString());

    // WHEN switching again
    mMessageLineWidget->setText(QStringLiteral("Text for room 1"));

    // THEN the other text should appear again
    w.setChannelSelected(room2->roomId(), room2->channelType());
    QCOMPARE(mMessageLineWidget->text(), QStringLiteral("Text for room 1"));
}

void RoomWidgetTest::shouldShowNoticeWhenReplyingToThread()
{
    QEventLoop loop;
    RoomWidget w;

    auto mRoomReplyThreadWidget = w.findChild<RoomReplyThreadWidget *>(QStringLiteral("mRoomReplyThreadWidget"));
    QVERIFY(!mRoomReplyThreadWidget->isVisible());

    auto mMessageLineWidget = w.findChild<MessageLineWidget *>(QStringLiteral("mMessageLineWidget"));
    QVERIFY(mMessageLineWidget);

    mMessageLineWidget->setThreadMessageId(QStringLiteral("placeholder"));
    loop.processEvents();
    QVERIFY(!mRoomReplyThreadWidget->isHidden());

    mMessageLineWidget->setThreadMessageId({});
    loop.processEvents();
    QVERIFY(mRoomReplyThreadWidget->isHidden());
}
