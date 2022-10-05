/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userrequestdatadownloadjobtest.h"
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
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.requestDataDownload")));
}
