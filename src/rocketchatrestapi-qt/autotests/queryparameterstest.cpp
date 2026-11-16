/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "queryparameterstest.h"

#include "queryparameters.h"
#include <QTest>

#include <QUrlQuery>

using namespace Qt::Literals::StringLiterals;

QTEST_GUILESS_MAIN(QueryParametersTest)

QueryParametersTest::QueryParametersTest(QObject *parent)
    : QObject(parent)
{
}

void QueryParametersTest::shouldHaveDefaultValue()
{
    const RocketChatRestApi::QueryParameters query;
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
        query.setFilter(u"bla"_s);
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
        query.setSearchString(u"bla"_s);
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
        QCOMPARE(urlQuery.toString(), u"count=4"_s);
    }
    {
        RocketChatRestApi::QueryParameters query;
        QMap<QString, QString> custom;
        custom.insert(u"bla"_s, u"foo"_s);
        query.setCustom(custom);
        QVERIFY(query.isValid());
        QUrlQuery urlQuery;
        RocketChatRestApi::QueryParameters::generateQueryParameter(query, urlQuery);
        QCOMPARE(urlQuery.toString(), u"bla=foo"_s);
    }
    {
        RocketChatRestApi::QueryParameters query;
        QVERIFY(!query.isValid());
        QMap<QString, QString> custom;
        custom.insert(u"text"_s, u"gene"_s);
        custom.insert(u"type"_s, u"channels"_s);
        query.setCustom(custom);
        QVERIFY(query.isValid());
        QUrlQuery urlQuery;
        RocketChatRestApi::QueryParameters::generateQueryParameter(query, urlQuery);
        QCOMPARE(urlQuery.toString(), u"text=gene&type=channels"_s);
    }
    {
        RocketChatRestApi::QueryParameters query;
        query.setSearchString(u"blabla"_s);
        QVERIFY(query.isValid());
        QUrlQuery urlQuery;
        RocketChatRestApi::QueryParameters::generateQueryParameter(query, urlQuery);
        QCOMPARE(urlQuery.toString(), u"name=blabla"_s);
    }
}

#include "moc_queryparameterstest.cpp"
