/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "queryparameterstest.h"
using namespace Qt::Literals::StringLiterals;

#include "queryparameters.h"
#include "restapimethod.h"
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
    QVERIFY(!query.useSyntaxRc70());
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
        query.setUseSyntaxRc70(false);
        QVERIFY(!query.isValid());
        query.setCount(4);
        QUrlQuery urlQuery;
        RocketChatRestApi::QueryParameters::generateQueryParameter(query, urlQuery);
        QCOMPARE(urlQuery.toString(), u"count=4"_s);

        urlQuery.clear();
        query.setUseSyntaxRc70(true);
        RocketChatRestApi::QueryParameters::generateQueryParameter(query, urlQuery);
        QCOMPARE(urlQuery.toString(), u"count=4"_s);
    }
    {
        RocketChatRestApi::QueryParameters query;
        query.setUseSyntaxRc70(false);
        QVERIFY(!query.isValid());
        QMap<QString, QString> custom;
        custom.insert(u"bla"_s, u"foo"_s);
        query.setCustom(custom);
        QVERIFY(query.isValid());
        QUrlQuery urlQuery;
        RocketChatRestApi::QueryParameters::generateQueryParameter(query, urlQuery);
        QCOMPARE(urlQuery.toString(), u"query=%7B%22bla%22:%22foo%22%7D"_s);

        urlQuery.clear();
        query.setUseSyntaxRc70(true);
        RocketChatRestApi::QueryParameters::generateQueryParameter(query, urlQuery);
        QCOMPARE(urlQuery.toString(), u"bla=foo"_s);
    }
    {
        RocketChatRestApi::QueryParameters query;
        query.setUseSyntaxRc70(false);
        QVERIFY(!query.isValid());
        QMap<QString, QString> custom;
        custom.insert(u"text"_s, u"gene"_s);
        custom.insert(u"type"_s, u"channels"_s);
        query.setCustom(custom);
        QVERIFY(query.isValid());
        QUrlQuery urlQuery;
        RocketChatRestApi::QueryParameters::generateQueryParameter(query, urlQuery);
        QCOMPARE(urlQuery.toString(), u"query=%7B%22text%22:%22gene%22,%22type%22:%22channels%22%7D"_s);

        urlQuery.clear();
        query.setUseSyntaxRc70(true);
        RocketChatRestApi::QueryParameters::generateQueryParameter(query, urlQuery);
        QCOMPARE(urlQuery.toString(), u"text=gene&type=channels"_s);
    }
    {
        RocketChatRestApi::QueryParameters query;
        query.setUseSyntaxRc70(false);
        QVERIFY(!query.isValid());
        query.setSearchString(u"blabla"_s);
        QVERIFY(query.isValid());
        QUrlQuery urlQuery;
        RocketChatRestApi::QueryParameters::generateQueryParameter(query, urlQuery);
        QCOMPARE(urlQuery.toString(), u"query=%7B%22name%22:%7B%22$regex%22:%22blabla%22,%22$options%22:%22i%22%7D%7D"_s);

        urlQuery.clear();
        query.setUseSyntaxRc70(true);
        RocketChatRestApi::QueryParameters::generateQueryParameter(query, urlQuery);
        QCOMPARE(urlQuery.toString(), u"name=blabla"_s);
    }
}

#include "moc_queryparameterstest.cpp"
