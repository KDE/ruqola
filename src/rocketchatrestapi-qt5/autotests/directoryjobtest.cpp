/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "directoryjobtest.h"
#include "misc/directoryjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(DirectoryJobTest)
using namespace RocketChatRestApi;
DirectoryJobTest::DirectoryJobTest(QObject *parent)
    : QObject(parent)
{
}

void DirectoryJobTest::shouldHaveDefaultValue()
{
    DirectoryJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.hasQueryParameterSupport());
}

void DirectoryJobTest::shouldGenerateRequest()
{
    DirectoryJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/directory")));
    }
}

void DirectoryJobTest::shouldNotStarting()
{
    DirectoryJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager networkAccessManager;
    job.setNetworkAccessManager(&networkAccessManager);
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    DirectoryJob::DirectoryInfo info;
    info.searchType = DirectoryJob::Rooms;
    job.setDirectoryInfo(info);
    QVERIFY(job.canStart());
}
