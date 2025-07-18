/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "channelusercompletertest.h"
using namespace Qt::Literals::StringLiterals;

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
    QVERIFY(w.fName().isEmpty());
    QVERIFY(w.userName().isEmpty());
    QVERIFY(w.avatarTag().isEmpty());
    QCOMPARE(w.type(), ChannelUserCompleter::ChannelUserCompleterType::Unknown);
    QVERIFY(w.statusIcon().isNull());
    QVERIFY(!w.outsideRoom());
    QVERIFY(w.identifier().isEmpty());
}

void ChannelUserCompleterTest::shouldReturnCompleterName()
{
    ChannelUserCompleter w;
    const QString name{u"name"_s};
    const QString userName{u"userName"_s};
    w.setName(name);
    w.setUserName(userName);
    w.setType(ChannelUserCompleter::ChannelUserCompleterType::Notification);
    QCOMPARE(w.completerName(), name);
    w.setType(ChannelUserCompleter::ChannelUserCompleterType::DirectChannel);
    QCOMPARE(w.completerName(), userName);
    w.setType(ChannelUserCompleter::ChannelUserCompleterType::Room);
    QCOMPARE(w.completerName(), name);
    w.setType(ChannelUserCompleter::ChannelUserCompleterType::Unknown);
    QCOMPARE(w.completerName(), QString());
}

#include "moc_channelusercompletertest.cpp"
