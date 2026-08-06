/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getusersofroomwithoutkeyjobtest.h"

#include "e2e/getusersofroomwithoutkeyjob.h"
#include "restapimethod.h"
#include <QTest>

QTEST_GUILESS_MAIN(GetUsersOfRoomWithoutKeyJobTest)
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
GetUsersOfRoomWithoutKeyJobTest::GetUsersOfRoomWithoutKeyJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetUsersOfRoomWithoutKeyJobTest::shouldHaveDefaultValue()
{
    GetUsersOfRoomWithoutKeyJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GetUsersOfRoomWithoutKeyJobTest::shouldGenerateRequest()
{
    GetUsersOfRoomWithoutKeyJob job;
    job.setRoomId("foo"_ba);
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/e2e.getUsersOfRoomWithoutKey?rid=foo"_s));
}

#include "moc_getusersofroomwithoutkeyjobtest.cpp"
