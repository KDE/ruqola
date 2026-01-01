/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelstest.h"
#include "messages/channels.h"
#include <QTest>
QTEST_GUILESS_MAIN(ChannelsTest)

ChannelsTest::ChannelsTest(QObject *parent)
    : QObject{parent}
{
}

void ChannelsTest::shouldHaveDefaultValues()
{
    Channels w;
    QVERIFY(w.isEmpty());
    QVERIFY(w.channels().isEmpty());
}

#include "moc_channelstest.cpp"
