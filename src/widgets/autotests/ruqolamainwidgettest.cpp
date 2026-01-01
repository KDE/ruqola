/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolamainwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#if 0
#include "bannerinfodialog/bannermessagewidget.h"
#endif
#include "channellist/channellistwidget.h"
#include "room/roomwidget.h"
#include "ruqolamainwidget.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QStackedWidget>
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
    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainlayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto topLayout = w.findChild<QVBoxLayout *>(u"topLayout"_s);
    QVERIFY(topLayout);
    QCOMPARE(topLayout->contentsMargins(), QMargins{});

    auto mSplitter = w.findChild<QSplitter *>(u"mSplitter"_s);
    QVERIFY(mSplitter);
    QVERIFY(!mSplitter->childrenCollapsible());

    auto mChannelList = w.findChild<ChannelListWidget *>(u"mChannelList"_s);
    QVERIFY(mChannelList);

    auto mStackedRoomWidget = w.findChild<QStackedWidget *>(u"mStackedRoomWidget"_s);
    QVERIFY(mStackedRoomWidget);

    auto mRoomWidget = w.findChild<RoomWidget *>(u"mRoomWidget"_s);
    QVERIFY(mRoomWidget);
    QVERIFY(mSplitter->indexOf(mChannelList) >= 0);
    QVERIFY(mSplitter->indexOf(mStackedRoomWidget) >= 0);

    auto mEmptyRoomWidget = w.findChild<QWidget *>(u"mEmptyRoomWidget"_s);
    QVERIFY(mEmptyRoomWidget);
    QCOMPARE(mStackedRoomWidget->currentWidget(), mEmptyRoomWidget);

#if 0
    auto mBannerMessageWidget = w.findChild<BannerMessageWidget *>(u"mBannerMessageWidget"_s);
    QVERIFY(mBannerMessageWidget);
#endif
}

#include "moc_ruqolamainwidgettest.cpp"
