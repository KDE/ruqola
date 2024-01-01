/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatemessagejobtest.h"
#include "autotranslate/translatemessagejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(TranslateMessageJobTest)
using namespace RocketChatRestApi;
TranslateMessageJobTest::TranslateMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void TranslateMessageJobTest::shouldHaveDefaultValue()
{
    TranslateMessageJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(job.targetLanguage().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void TranslateMessageJobTest::shouldGenerateRequest()
{
    TranslateMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/autotranslate.translateMessage")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void TranslateMessageJobTest::shouldGenerateJson()
{
    TranslateMessageJob job;
    const QString messageId = QStringLiteral("foo1");
    job.setMessageId(messageId);
    const QString targetLanguage = QStringLiteral("bla");
    job.setTargetLanguage(targetLanguage);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"messageId":"%1","targetLanguage":"%2"})").arg(messageId, targetLanguage).toLatin1());
}

void TranslateMessageJobTest::shouldNotStarting()
{
    TranslateMessageJob job;

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
    QVERIFY(!job.canStart());
    const QString targetLanguage = QStringLiteral("bla");
    job.setTargetLanguage(targetLanguage);
    QVERIFY(job.canStart());
}

#include "moc_translatemessagejobtest.cpp"
