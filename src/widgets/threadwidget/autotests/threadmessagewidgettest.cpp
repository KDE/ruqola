/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "threadmessagewidgettest.h"
#include "room/messagelinewidget.h"
#include "room/messagelistview.h"
#include "room/roomwidgetbase.h"
#include "threadwidget/threadmessagewidget.h"
#include <QLabel>
#include <QStandardPaths>
#include <QTest>
#include <QToolButton>
#include <QVBoxLayout>
QTEST_MAIN(ThreadMessageWidgetTest)

ThreadMessageWidgetTest::ThreadMessageWidgetTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ThreadMessageWidgetTest::shouldHaveDefaultValues()
{
    ThreadMessageWidget::ThreadMessageInfo info;
    QVERIFY(info.threadMessageId.isEmpty());
    QVERIFY(info.threadMessagePreview.isEmpty());
    QVERIFY(!info.threadIsFollowing);
    QVERIFY(!info.room);

    ThreadMessageWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mThreadPreview = w.findChild<QLabel *>(QStringLiteral("mThreadPreview"));
    QVERIFY(mThreadPreview);
    QVERIFY(mThreadPreview->wordWrap());
    QCOMPARE(mThreadPreview->contextMenuPolicy(), Qt::NoContextMenu);
    QVERIFY(mThreadPreview->text().isEmpty());

    auto mRoomWidgetBase = w.findChild<RoomWidgetBase *>(QStringLiteral("mRoomWidgetBase"));
    QVERIFY(mRoomWidgetBase);

    auto hboxLayout = w.findChild<QHBoxLayout *>(QStringLiteral("hboxLayout"));
    QVERIFY(hboxLayout);
    QCOMPARE(hboxLayout->contentsMargins(), QMargins());

    auto mFollowButton = w.findChild<QToolButton *>(QStringLiteral("mFollowButton"));
    QVERIFY(mFollowButton);
    QVERIFY(mFollowButton->autoRaise());
    QVERIFY(mFollowButton->isCheckable());
}

void ThreadMessageWidgetTest::shouldChangeThreadPreview()
{
    ThreadMessageWidget w(nullptr);
    auto mThreadPreview = w.findChild<QLabel *>(QStringLiteral("mThreadPreview"));
    const QString threadPreview{QStringLiteral("bla")};
    ThreadMessageWidget::ThreadMessageInfo info;
    info.threadMessagePreview = threadPreview;

    w.setThreadMessageInfo(info);
    QCOMPARE(mThreadPreview->text(), threadPreview);
}

#include "moc_threadmessagewidgettest.cpp"
