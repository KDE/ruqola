/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "threadmessagewidgettest.h"
#include "room/messagelinewidget.h"
#include "room/messagelistview.h"
#include "room/roomwidgetbase.h"
#include "threadwidget/threadmessagewidget.h"
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ThreadMessageWidgetTest)

ThreadMessageWidgetTest::ThreadMessageWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ThreadMessageWidgetTest::shouldHaveDefaultValues()
{
    ThreadMessageWidget w(nullptr);
    QVERIFY(w.threadMessageId().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto mThreadPreview = w.findChild<QLabel *>(QStringLiteral("mThreadPreview"));
    QVERIFY(mThreadPreview);
    QVERIFY(mThreadPreview->wordWrap());
    QCOMPARE(mThreadPreview->contextMenuPolicy(), Qt::CustomContextMenu);
    QVERIFY(mThreadPreview->text().isEmpty());

    auto mRoomWidgetBase = w.findChild<RoomWidgetBase *>(QStringLiteral("mRoomWidgetBase"));
    QVERIFY(mRoomWidgetBase);
}
