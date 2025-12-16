/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "unfollowmessagejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "chat/unfollowmessagejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(UnFollowMessageJobTest)
using namespace RocketChatRestApi;
UnFollowMessageJobTest::UnFollowMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void UnFollowMessageJobTest::shouldHaveDefaultValue()
{
    UnFollowMessageJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void UnFollowMessageJobTest::shouldGenerateRequest()
{
    UnFollowMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.unfollowMessage"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void UnFollowMessageJobTest::shouldGenerateJson()
{
    UnFollowMessageJob job;
    const QByteArray messageid("foo1");
    job.setMessageId(messageid);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"mid":"%1"})").arg(QLatin1StringView(messageid)).toLatin1());
}

void UnFollowMessageJobTest::shouldNotStarting()
{
    UnFollowMessageJob job;

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
    const QByteArray messageId("foo1");
    job.setMessageId(messageId);
    QVERIFY(job.canStart());
}

#include "moc_unfollowmessagejobtest.cpp"
