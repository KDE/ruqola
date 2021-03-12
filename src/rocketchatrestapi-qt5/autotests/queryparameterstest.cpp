/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
}

void QueryParametersTest::shouldQueryIsValid()
{
    RocketChatRestApi::QueryParameters query;
    QVERIFY(!query.isValid());
    query.setCount(4);
    QVERIFY(query.isValid());
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
}
