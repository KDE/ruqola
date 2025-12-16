/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userrequestdatadownloadjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "users/userrequestdatadownloadjob.h"
#include <QTest>

QTEST_GUILESS_MAIN(UserRequestDataDownloadJobTest)
using namespace RocketChatRestApi;
UserRequestDataDownloadJobTest::UserRequestDataDownloadJobTest(QObject *parent)
    : QObject(parent)
{
}

void UserRequestDataDownloadJobTest::shouldHaveDefaultValue()
{
    UserRequestDataDownloadJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void UserRequestDataDownloadJobTest::shouldGenerateRequest()
{
    UserRequestDataDownloadJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.requestDataDownload?fullExport=false"_s));

    job.setFullExport(true);
    request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.requestDataDownload?fullExport=true"_s));
}

#include "moc_userrequestdatadownloadjobtest.cpp"
