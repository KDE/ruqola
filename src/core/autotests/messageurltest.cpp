/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

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
    QVERIFY(url.imageUrl().isEmpty());
    QVERIFY(url.authorName().isEmpty());
    QVERIFY(url.authorUrl().isEmpty());
    QVERIFY(url.siteUrl().isEmpty());
    QVERIFY(url.urlId().isEmpty());
    QCOMPARE(url.imageHeight(), -1);
    QCOMPARE(url.imageWidth(), -1);
}

void MessageUrlTest::shouldSerializeData()
{
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setPageTitle(QStringLiteral("foo2"));
        input.setDescription(QStringLiteral("foo3"));
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setPageTitle(QStringLiteral("foo2"));
        input.setDescription(QStringLiteral("foo3"));
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo3"));
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo3"));
        input.setImageUrl(QStringLiteral("foo4"));
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo3"));
        input.setImageUrl(QStringLiteral("foo4"));
        input.setAuthorName(QStringLiteral("foo5"));
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo3"));
        input.setImageUrl(QStringLiteral("foo4"));
        input.setAuthorName(QStringLiteral("foo5"));
        input.setAuthorUrl(QStringLiteral("foo6"));
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo3"));
        input.setImageUrl(QStringLiteral("foo4"));
        input.setAuthorName(QStringLiteral("foo5"));
        input.setAuthorUrl(QStringLiteral("foo6"));
        input.setSiteUrl(QStringLiteral("foo7"));
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo3"));
        input.setImageUrl(QStringLiteral("foo4"));
        input.setAuthorName(QStringLiteral("foo5"));
        input.setAuthorUrl(QStringLiteral("foo6"));
        input.setSiteUrl(QStringLiteral("foo7"));
        input.setImageHeight(8);
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo3"));
        input.setImageUrl(QStringLiteral("foo4"));
        input.setAuthorName(QStringLiteral("foo5"));
        input.setAuthorUrl(QStringLiteral("foo6"));
        input.setSiteUrl(QStringLiteral("foo7"));
        input.setImageHeight(8);
        input.setImageWidth(32);
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
}

#include "moc_messageurltest.cpp"
