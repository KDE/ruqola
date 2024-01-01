/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasewidgettest.h"
#include "databasedialog/exploredatabasewidget.h"
#include "room/messagelistview.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ExploreDatabaseWidgetTest)
ExploreDatabaseWidgetTest::ExploreDatabaseWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ExploreDatabaseWidgetTest::shouldHaveDefaultValues()
{
    ExploreDatabaseWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto hboxLayout = w.findChild<QHBoxLayout *>(QStringLiteral("hboxLayout"));
    QVERIFY(hboxLayout);
    QCOMPARE(hboxLayout->contentsMargins(), QMargins{});

    auto mRoomName = w.findChild<QLineEdit *>(QStringLiteral("mRoomName"));
    QVERIFY(mRoomName);
    QVERIFY(mRoomName->isClearButtonEnabled());
    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto pushButton = w.findChild<QPushButton *>(QStringLiteral("pushButton"));
    QVERIFY(pushButton);
    QVERIFY(!pushButton->text().isEmpty());

    auto mMessageListView = w.findChild<MessageListView *>(QStringLiteral("mMessageListView"));
    QVERIFY(mMessageListView);
}

#include "moc_exploredatabasewidgettest.cpp"
