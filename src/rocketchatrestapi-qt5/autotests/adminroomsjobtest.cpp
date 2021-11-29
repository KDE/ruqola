/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminroomsjobtest.h"
#include "restapimethod.h"
#include "rooms/adminroomsjob.h"
#include <QTest>
QTEST_GUILESS_MAIN(AdminRoomsJobTest)
using namespace RocketChatRestApi;
AdminRoomsJobTest::AdminRoomsJobTest(QObject *parent)
    : QObject(parent)
{
}

void AdminRoomsJobTest::shouldHaveDefaultValue()
{
    AdminRoomsJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.hasQueryParameterSupport());
}

void AdminRoomsJobTest::shouldGenerateRequest()
{
    AdminRoomsJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.adminRooms")));
    AdminRoomsJob::AdminRoomsJobInfo info;
    info.filter = QStringLiteral("foo");
    job.setRoomsAdminInfo(info);
    request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.adminRooms?filter=%1").arg(info.filter)));
}
