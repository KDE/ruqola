/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

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
    ThreadMessageWidget w(nullptr);
    QVERIFY(w.threadMessageId().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mThreadPreview = w.findChild<QLabel *>(QStringLiteral("mThreadPreview"));
    QVERIFY(mThreadPreview);
    QVERIFY(mThreadPreview->wordWrap());
    QCOMPARE(mThreadPreview->contextMenuPolicy(), Qt::CustomContextMenu);
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
    w.setThreadPreview(threadPreview);
    QCOMPARE(mThreadPreview->text(), threadPreview);
}
