/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolamainwidgettest.h"
#include "bannerinfodialog/bannermessagewidget.h"
#include "channellist/channellistwidget.h"
#include "room/roomwidget.h"
#include "ruqolamainwidget.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QStandardPaths>
#include <QTest>

QTEST_MAIN(RuqolaMainWidgetTest)

RuqolaMainWidgetTest::RuqolaMainWidgetTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void RuqolaMainWidgetTest::shouldHaveDefaultValues()
{
    RuqolaMainWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto topLayout = w.findChild<QVBoxLayout *>(QStringLiteral("topLayout"));
    QVERIFY(topLayout);
    QCOMPARE(topLayout->contentsMargins(), QMargins{});

    auto mSplitter = w.findChild<QSplitter *>(QStringLiteral("mSplitter"));
    QVERIFY(mSplitter);
    QVERIFY(!mSplitter->childrenCollapsible());

    auto mChannelList = w.findChild<ChannelListWidget *>(QStringLiteral("mChannelList"));
    QVERIFY(mChannelList);

    auto mStackedRoomWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedRoomWidget"));
    QVERIFY(mStackedRoomWidget);

    auto mRoomWidget = w.findChild<RoomWidget *>(QStringLiteral("mRoomWidget"));
    QVERIFY(mRoomWidget);
    QVERIFY(mSplitter->indexOf(mChannelList) >= 0);
    QVERIFY(mSplitter->indexOf(mStackedRoomWidget) >= 0);

    auto mEmptyRoomWidget = w.findChild<QWidget *>(QStringLiteral("mEmptyRoomWidget"));
    QVERIFY(mEmptyRoomWidget);
    QCOMPARE(mStackedRoomWidget->currentWidget(), mEmptyRoomWidget);

    auto mBannerMessageWidget = w.findChild<BannerMessageWidget *>(QStringLiteral("mBannerMessageWidget"));
    QVERIFY(mBannerMessageWidget);
}
