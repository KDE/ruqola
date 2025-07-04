/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createdmjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "directmessage/createdmjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
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
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/im.create"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void CreateDmJobTest::shouldGenerateJson()
{
    CreateDmJob job;
    {
        const QString channelname = u"foo1"_s;
        job.setUserNames({channelname});
        QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"username":"%1"})").arg(channelname).toLatin1());
    }
    {
        const QStringList userNames = {u"foo1"_s, u"bla"_s, QStringLiteral("bli")};
        job.setUserNames(userNames);
        QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"usernames":"foo1,bla,bli"})").toLatin1());
    }
}

void CreateDmJobTest::shouldNotStarting()
{
    CreateDmJob job;

    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString username = u"foo1"_s;
    job.setUserNames({username});
    QVERIFY(job.canStart());
}

#include "moc_createdmjobtest.cpp"
