/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomwidgetbasetest.h"
#include "room/messagelinewidget.h"
#include "room/readonlylineeditwidget.h"
#include "room/roomquotemessagewidget.h"
#include "room/roomreplythreadwidget.h"
#include "room/roomwidgetbase.h"
#include "room/uploadfileprogressstatuslistwidget.h"
#include <QStackedWidget>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(RoomWidgetBaseTest)

RoomWidgetBaseTest::RoomWidgetBaseTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void RoomWidgetBaseTest::shouldHaveDefaultValues()
{
    RoomWidgetBase w(MessageListView::Mode::Editing);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mUploadFileProgressStatusListWidget = w.findChild<UploadFileProgressStatusListWidget *>(QStringLiteral("mUploadFileProgressStatusListWidget"));
    QVERIFY(mUploadFileProgressStatusListWidget);

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

    auto mRoomReplyThreadWidget = w.findChild<RoomReplyThreadWidget *>(QStringLiteral("mRoomReplyThreadWidget"));
    QVERIFY(mRoomReplyThreadWidget);
    QVERIFY(!mRoomReplyThreadWidget->isVisible());

    auto mRoomQuoteMessageWidget = w.findChild<RoomQuoteMessageWidget *>(QStringLiteral("mRoomQuoteMessageWidget"));
    QVERIFY(mRoomQuoteMessageWidget);
    QVERIFY(!mRoomQuoteMessageWidget->isVisible());
}

#include "moc_roomwidgetbasetest.cpp"
