/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageurltest.h"
#include "messages/messageurl.h"
#include <QJsonObject>
#include <QTest>
QTEST_GUILESS_MAIN(MessageUrlTest)

MessageUrlTest::MessageUrlTest(QObject *parent)
    : QObject(parent)
{
}

void MessageUrlTest::shouldHaveDefaultValue()
{
    MessageUrl url;
    QVERIFY(url.url().isEmpty());
    QVERIFY(url.pageTitle().isEmpty());
    QVERIFY(url.description().isEmpty());
    QVERIFY(url.isEmpty());
}

void MessageUrlTest::shouldSerializeData()
{
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setPageTitle(QStringLiteral("foo2"));
        input.setDescription(QStringLiteral("foo3"));
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::fromJSon(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setPageTitle(QStringLiteral("foo2"));
        input.setDescription(QStringLiteral("foo3"));
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::fromJSon(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo3"));
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::fromJSon(ba);
        QCOMPARE(input, output);
    }
}
