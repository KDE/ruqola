/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channellistwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainlayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mSearchRoom = w.findChild<QLineEdit *>(u"mSearchRoom"_s);
    QVERIFY(mSearchRoom);
    QVERIFY(!mSearchRoom->placeholderText().isEmpty());

    auto mChannelView = w.findChild<ChannelListView *>(u"mChannelView"_s);
    QVERIFY(mChannelView);
}

void ChannelListWidgetTest::searchRooms()
{
    // LAurent: We don't have model by default now. We need to fix it.
    //    ChannelListWidget w;

    //    auto mSearchRoom = w.findChild<QLineEdit *>(u"mSearchRoom"_s);
    //    QVERIFY(mSearchRoom);
    //    mSearchRoom->setText(u"all"_s); // triggers filtering of the channel list

    //    QVERIFY(w.channelListView());
    //    QVERIFY(w.channelListView()->model());
    //    QCOMPARE(w.channelListView()->model()->filterRegExp().pattern(), u"all"_s);
}

#include "moc_channellistwidgettest.cpp"
