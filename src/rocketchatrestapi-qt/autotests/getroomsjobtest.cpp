/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getroomsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rooms/getroomsjob.h"
#include <QTest>
QTEST_GUILESS_MAIN(GetRoomsJobTest)
using namespace RocketChatRestApi;
GetRoomsJobTest::GetRoomsJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetRoomsJobTest::shouldHaveDefaultValue()
{
    GetRoomsJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void GetRoomsJobTest::shouldGenerateRequest()
{
    GetRoomsJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.get"_s));
}

#include "moc_getroomsjobtest.cpp"
