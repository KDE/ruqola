/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "followmessagejobtest.h"
#include "chat/followmessagejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(FollowMessageJobTest)
using namespace RocketChatRestApi;
FollowMessageJobTest::FollowMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void FollowMessageJobTest::shouldHaveDefaultValue()
{
    FollowMessageJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void FollowMessageJobTest::shouldGenerateRequest()
{
    FollowMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.followMessage")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void FollowMessageJobTest::shouldGenerateJson()
{
    FollowMessageJob job;
    const QString messageid = QStringLiteral("foo1");
    job.setMessageId(messageid);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"mid":"%1"})").arg(messageid).toLatin1());
}

void FollowMessageJobTest::shouldNotStarting()
{
    FollowMessageJob job;

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
    const QString messageId = QStringLiteral("foo1");
    job.setMessageId(messageId);
    QVERIFY(job.canStart());
}

#include "moc_followmessagejobtest.cpp"
