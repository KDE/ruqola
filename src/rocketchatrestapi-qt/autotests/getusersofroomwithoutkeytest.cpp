/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getusersofroomwithoutkeytest.h"
using namespace Qt::Literals::StringLiterals;

#include "e2e/getusersofroomwithoutkey.h"
#include "restapimethod.h"
#include <QTest>

QTEST_GUILESS_MAIN(GetUsersOfRoomWithoutKeyTest)
using namespace RocketChatRestApi;
GetUsersOfRoomWithoutKeyTest::GetUsersOfRoomWithoutKeyTest(QObject *parent)
    : QObject(parent)
{
}

void GetUsersOfRoomWithoutKeyTest::shouldHaveDefaultValue()
{
    GetUsersOfRoomWithoutKey job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GetUsersOfRoomWithoutKeyTest::shouldGenerateRequest()
{
    GetUsersOfRoomWithoutKey job;
    job.setRoomId("foo"_ba);
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/e2e.getUsersOfRoomWithoutKey?roomId=foo"_s));
}

#include "moc_getusersofroomwithoutkeytest.cpp"
