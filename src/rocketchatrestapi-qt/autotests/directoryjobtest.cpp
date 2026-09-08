/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directoryjobtest.h"
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
    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    DirectoryJob::DirectoryInfo info;
    info.searchType = DirectoryJob::SearchType::Rooms;
    job.setDirectoryInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_directoryjobtest.cpp"
