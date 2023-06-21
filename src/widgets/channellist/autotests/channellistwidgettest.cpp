/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channellistwidgettest.h"
#include "channellist/channellistview.h"
#include "channellist/channellistwidget.h"
#include "model/roomfilterproxymodel.h"

#include <QLineEdit>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ChannelListWidgetTest)
ChannelListWidgetTest::ChannelListWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelListWidgetTest::shouldHaveDefaultValues()
{
    ChannelListWidget w;

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mSearchRoom = w.findChild<QLineEdit *>(QStringLiteral("mSearchRoom"));
    QVERIFY(mSearchRoom);
    QVERIFY(!mSearchRoom->placeholderText().isEmpty());

    auto mChannelView = w.findChild<ChannelListView *>(QStringLiteral("mChannelView"));
    QVERIFY(mChannelView);
}

void ChannelListWidgetTest::searchRooms()
{
    // LAurent: We don't have model by default now. We need to fix it.
    //    ChannelListWidget w;

    //    auto mSearchRoom = w.findChild<QLineEdit *>(QStringLiteral("mSearchRoom"));
    //    QVERIFY(mSearchRoom);
    //    mSearchRoom->setText(QStringLiteral("all")); // triggers filtering of the channel list

    //    QVERIFY(w.channelListView());
    //    QVERIFY(w.channelListView()->model());
    //    QCOMPARE(w.channelListView()->model()->filterRegExp().pattern(), QStringLiteral("all"));
}

#include "moc_channellistwidgettest.cpp"
