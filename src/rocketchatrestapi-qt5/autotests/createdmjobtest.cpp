/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createdmjobtest.h"
#include "directmessage/createdmjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(CreateDmJobTest)
using namespace RocketChatRestApi;
CreateDmJobTest::CreateDmJobTest(QObject *parent)
    : QObject(parent)
{
}

void CreateDmJobTest::shouldHaveDefaultValue()
{
    CreateDmJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.userNames().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void CreateDmJobTest::shouldGenerateRequest()
{
    CreateDmJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/im.create")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void CreateDmJobTest::shouldGenerateJson()
{
    CreateDmJob job;
    {
        const QString channelname = QStringLiteral("foo1");
        job.setUserNames({channelname});
        QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"username":"%1"})").arg(channelname).toLatin1());
    }
    {
        const QStringList userNames = {QStringLiteral("foo1"), QStringLiteral("bla"), QStringLiteral("bli")};
        job.setUserNames(userNames);
        QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"usernames":"foo1,bla,bli"})").toLatin1());
    }
}

void CreateDmJobTest::shouldNotStarting()
{
    CreateDmJob job;

    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString username = QStringLiteral("foo1");
    job.setUserNames({username});
    QVERIFY(job.canStart());
}
