/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

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
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(ExploreDatabaseWidgetTest)
ExploreDatabaseWidgetTest::ExploreDatabaseWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ExploreDatabaseWidgetTest::shouldHaveDefaultValues()
{
    ExploreDatabaseWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto hboxLayout = w.findChild<QHBoxLayout *>(u"hboxLayout"_s);
    QVERIFY(hboxLayout);
    QCOMPARE(hboxLayout->contentsMargins(), QMargins{});

    auto mRoomName = w.findChild<QLineEdit *>(u"mRoomName"_s);
    QVERIFY(mRoomName);
    QVERIFY(mRoomName->isClearButtonEnabled());
    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto pushButton = w.findChild<QPushButton *>(u"pushButton"_s);
    QVERIFY(pushButton);
    QVERIFY(!pushButton->text().isEmpty());

    auto mMessageListView = w.findChild<MessageListView *>(u"mMessageListView"_s);
    QVERIFY(mMessageListView);
}

#include "moc_exploredatabasewidgettest.cpp"
