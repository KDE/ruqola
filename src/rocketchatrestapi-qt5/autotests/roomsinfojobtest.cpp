/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsinfojobtest.h"
#include "restapimethod.h"
#include "rooms/roomsinfojob.h"
#include <QTest>
QTEST_GUILESS_MAIN(RoomsInfoJobTest)
using namespace RocketChatRestApi;
RoomsInfoJobTest::RoomsInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomsInfoJobTest::shouldHaveDefaultValue()
{
    RoomsInfoJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void RoomsInfoJobTest::shouldGenerateRequest()
{
    RoomsInfoJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.info")));
}
