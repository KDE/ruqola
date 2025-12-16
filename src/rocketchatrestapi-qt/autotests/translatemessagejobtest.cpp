/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatemessagejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "autotranslate/translatemessagejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(TranslateMessageJobTest)
using namespace RocketChatRestApi;
TranslateMessageJobTest::TranslateMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void TranslateMessageJobTest::shouldHaveDefaultValue()
{
    TranslateMessageJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
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
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/autotranslate.translateMessage"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void TranslateMessageJobTest::shouldGenerateJson()
{
    TranslateMessageJob job;
    const QString messageId = u"foo1"_s;
    job.setMessageId(messageId);
    const QString targetLanguage = u"bla"_s;
    job.setTargetLanguage(targetLanguage);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"messageId":"%1","targetLanguage":"%2"})").arg(messageId, targetLanguage).toLatin1());
}

void TranslateMessageJobTest::shouldNotStarting()
{
    TranslateMessageJob job;

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
    const QString messageId = u"foo1"_s;
    job.setMessageId(messageId);
    QVERIFY(!job.canStart());
    const QString targetLanguage = u"bla"_s;
    job.setTargetLanguage(targetLanguage);
    QVERIFY(job.canStart());
}

#include "moc_translatemessagejobtest.cpp"
