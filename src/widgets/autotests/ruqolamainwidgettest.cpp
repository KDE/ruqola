/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "ruqolamainwidgettest.h"
#include "ruqolamainwidget.h"
#include "channellist/channellistwidget.h"
#include "room/roomwidget.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QTest>

QTEST_MAIN(RuqolaMainWidgetTest)

RuqolaMainWidgetTest::RuqolaMainWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void RuqolaMainWidgetTest::shouldHaveDefaultValues()
{
    RuqolaMainWidget w;
    QHBoxLayout *mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);

    QSplitter *mSplitter = w.findChild<QSplitter *>(QStringLiteral("mSplitter"));
    QVERIFY(mSplitter);
    QVERIFY(!mSplitter->childrenCollapsible());

    ChannelListWidget *mChannelList = w.findChild<ChannelListWidget *>(QStringLiteral("mChannelList"));
    QVERIFY(mChannelList);

    QStackedWidget *mStackedRoomWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedRoomWidget"));
    QVERIFY(mStackedRoomWidget);

    RoomWidget *mRoomWidget = w.findChild<RoomWidget *>(QStringLiteral("mRoomWidget"));
    QVERIFY(mRoomWidget);
    QVERIFY(mSplitter->indexOf(mChannelList) >= 0);
    QVERIFY(mSplitter->indexOf(mStackedRoomWidget) >= 0);


    QWidget *mEmptyRoomWidget = w.findChild<QWidget *>(QStringLiteral("mEmptyRoomWidget"));
    QVERIFY(mEmptyRoomWidget);
    QCOMPARE(mStackedRoomWidget->currentWidget(), mEmptyRoomWidget);
}
