/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directoryjobtest.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/directoryjob.h"

#include "ruqola_restapi_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(DirectoryJobTest)
using namespace RocketChatRestApi;
DirectoryJobTest::DirectoryJobTest(QObject *parent)
    : QObject(parent)
{
}

void DirectoryJobTest::shouldHaveDefaultValue()
{
    DirectoryJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void DirectoryJobTest::shouldGenerateRequest()
{
    DirectoryJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/directory"_s));
    }
}

void DirectoryJobTest::shouldNotStarting()
{
    DirectoryJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);

    QNetworkAccessManager networkAccessManager;
    job.setNetworkAccessManager(&networkAccessManager);
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    DirectoryJob::DirectoryInfo info;
    info.searchType = DirectoryJob::SearchType::Rooms;
    job.setDirectoryInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_directoryjobtest.cpp"
