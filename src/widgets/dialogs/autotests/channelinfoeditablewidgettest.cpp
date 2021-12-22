/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinfoeditablewidgettest.h"
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
    ChannelInfoEditableWidget w(nullptr);
    auto layout = w.findChild<QFormLayout *>(QStringLiteral("layout"));
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins{});

    auto mRoomAvatarWidget = w.findChild<RoomAvatarWidget *>(QStringLiteral("mRoomAvatarWidget"));
    QVERIFY(mRoomAvatarWidget);
}
