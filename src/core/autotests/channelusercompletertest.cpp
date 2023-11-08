/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "channelusercompletertest.h"
#include "channelusercompleter.h"
#include <QTest>
QTEST_GUILESS_MAIN(ChannelUserCompleterTest)

ChannelUserCompleterTest::ChannelUserCompleterTest(QObject *parent)
    : QObject{parent}
{
}

void ChannelUserCompleterTest::shouldHaveDefaultValues()
{
    ChannelUserCompleter w;
    QVERIFY(w.description().isEmpty());
    QVERIFY(w.name().isEmpty());
    QVERIFY(w.userName().isEmpty());
    QVERIFY(w.avatarTag().isEmpty());
    QCOMPARE(w.type(), ChannelUserCompleter::ChannelUserCompleterType::Unknown);
    QVERIFY(w.statusIcon().isNull());
    QVERIFY(w.channelUserIcon().isNull());
    QVERIFY(!w.outsideRoom());
}

#include "moc_channelusercompletertest.cpp"
