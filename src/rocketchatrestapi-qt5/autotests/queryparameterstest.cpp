/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "queryparameterstest.h"
#include "restapiabstractjob.h"
#include <QTest>
#include <QUrlQuery>
QTEST_GUILESS_MAIN(QueryParametersTest)

QueryParametersTest::QueryParametersTest(QObject *parent)
    : QObject(parent)
{
}

void QueryParametersTest::shouldHaveDefaultValue()
{
    RocketChatRestApi::QueryParameters query;
    QCOMPARE(query.count(), -1);
    QCOMPARE(query.offset(), -1);
    QVERIFY(!query.isValid());
    QVERIFY(query.sorting().isEmpty());
    QVERIFY(query.custom().isEmpty());
    QVERIFY(query.searchString().isEmpty());
    QVERIFY(query.filter().isEmpty());
}

void QueryParametersTest::shouldQueryIsValid()
{
    {
        RocketChatRestApi::QueryParameters query;
        QVERIFY(!query.isValid());
        query.setCount(4);
        QVERIFY(query.isValid());
    }
    {
        RocketChatRestApi::QueryParameters query;
        QVERIFY(!query.isValid());
        query.setFilter(QStringLiteral("bla"));
        QVERIFY(query.isValid());
    }
    {
        RocketChatRestApi::QueryParameters query;
        QVERIFY(!query.isValid());
        query.setOffset(5);
        QVERIFY(query.isValid());
    }
    {
        RocketChatRestApi::QueryParameters query;
        QVERIFY(!query.isValid());
        query.setSearchString(QStringLiteral("bla"));
        QVERIFY(query.isValid());
    }
}

void QueryParametersTest::shouldGenerateQuery()
{
    {
        RocketChatRestApi::QueryParameters query;
        QVERIFY(!query.isValid());
        query.setCount(4);
        QUrlQuery urlQuery;
        RocketChatRestApi::QueryParameters::generateQueryParameter(query, urlQuery);
        QCOMPARE(urlQuery.toString(), QStringLiteral("count=4"));
    }
    {
        RocketChatRestApi::QueryParameters query;
        QVERIFY(!query.isValid());
        QMap<QString, QString> custom;
        custom.insert(QStringLiteral("bla"), QStringLiteral("foo"));
        query.setCustom(custom);
        QVERIFY(query.isValid());
        QUrlQuery urlQuery;
        RocketChatRestApi::QueryParameters::generateQueryParameter(query, urlQuery);
        QCOMPARE(urlQuery.toString(), QStringLiteral("query=%7B%22bla%22:%22foo%22%7D"));
    }
    {
        RocketChatRestApi::QueryParameters query;
        QVERIFY(!query.isValid());
        QMap<QString, QString> custom;
        custom.insert(QStringLiteral("text"), QStringLiteral("gene"));
        custom.insert(QStringLiteral("type"), QStringLiteral("channels"));
        query.setCustom(custom);
        QVERIFY(query.isValid());
        QUrlQuery urlQuery;
        RocketChatRestApi::QueryParameters::generateQueryParameter(query, urlQuery);
        QCOMPARE(urlQuery.toString(), QStringLiteral("query=%7B%22text%22:%22gene%22,%22type%22:%22channels%22%7D"));
    }
    {
        RocketChatRestApi::QueryParameters query;
        QVERIFY(!query.isValid());
        query.setSearchString(QStringLiteral("blabla"));
        QVERIFY(query.isValid());
        QUrlQuery urlQuery;
        RocketChatRestApi::QueryParameters::generateQueryParameter(query, urlQuery);
        QCOMPARE(urlQuery.toString(), QStringLiteral("query=%7B%22name%22:%7B%22$regex%22:%22blabla%22,%22$options%22:%22i%22%7D%7D"));
    }
}
