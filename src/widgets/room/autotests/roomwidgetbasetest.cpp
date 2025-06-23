/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomwidgetbasetest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mUploadFileProgressStatusListWidget = w.findChild<UploadFileProgressStatusListWidget *>(u"mUploadFileProgressStatusListWidget"_s);
    QVERIFY(mUploadFileProgressStatusListWidget);

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

    auto mRoomReplyThreadWidget = w.findChild<RoomReplyThreadWidget *>(u"mRoomReplyThreadWidget"_s);
    QVERIFY(mRoomReplyThreadWidget);
    QVERIFY(!mRoomReplyThreadWidget->isVisible());

    auto mRoomQuoteMessageWidget = w.findChild<RoomQuoteMessageWidget *>(u"mRoomQuoteMessageWidget"_s);
    QVERIFY(mRoomQuoteMessageWidget);
    QVERIFY(!mRoomQuoteMessageWidget->isVisible());
}

#include "moc_roomwidgetbasetest.cpp"
