/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "fetchuserswaitingforgroupkeyjobtest.h"

#include "e2e/fetchuserswaitingforgroupkeyjob.h"
#include "restapimethod.h"
#include <QTest>

QTEST_GUILESS_MAIN(FetchUsersWaitingForGroupKeyJobTest)
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
FetchUsersWaitingForGroupKeyJobTest::FetchUsersWaitingForGroupKeyJobTest(QObject *parent)
    : QObject(parent)
{
}

void FetchUsersWaitingForGroupKeyJobTest::shouldHaveDefaultValue()
{
    FetchUsersWaitingForGroupKeyJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void FetchUsersWaitingForGroupKeyJobTest::shouldGenerateRequest()
{
    FetchUsersWaitingForGroupKeyJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/e2e.fetchUsersWaitingForGroupKey"_s));
}

#include "moc_fetchuserswaitingforgroupkeyjobtest.cpp"
