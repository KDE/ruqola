/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinfoeditablewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/channelinfoeditablewidget.h"
#include "dialogs/roomavatarwidget.h"
#include <QFormLayout>
#include <QTest>
QTEST_MAIN(ChannelInfoEditableWidgetTest)
ChannelInfoEditableWidgetTest::ChannelInfoEditableWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelInfoEditableWidgetTest::shouldHaveDefaultValues()
{
    ChannelInfoEditableWidget w(nullptr, nullptr);
    auto layout = w.findChild<QFormLayout *>(u"layout"_s);
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins{});

    auto mRoomAvatarWidget = w.findChild<RoomAvatarWidget *>(u"mRoomAvatarWidget"_s);
    QVERIFY(mRoomAvatarWidget);
}

#include "moc_channelinfoeditablewidgettest.cpp"
