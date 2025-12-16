/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "methodcalljobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/methodcalljob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(MethodCallJobTest)
using namespace RocketChatRestApi;
MethodCallJobTest::MethodCallJobTest(QObject *parent)
    : QObject(parent)
{
}

void MethodCallJobTest::shouldHaveDefaultValue()
{
    MethodCallJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    MethodCallJob::MethodCallJobInfo info;
    info.anonymous = true;
    job.setMethodCallJobInfo(info);
    QVERIFY(!job.requireHttpAuthentication());
}

void MethodCallJobTest::shouldGenerateRequest()
{
    MethodCallJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/method.call"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);

    MethodCallJob::MethodCallJobInfo info;
    info.methodName = u"login"_s;
    info.anonymous = true;
    job.setMethodCallJobInfo(info);
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/method.callAnon/login"_s));
}

void MethodCallJobTest::shouldGenerateJson()
{
    MethodCallJob job;
    MethodCallJob::MethodCallJobInfo info;
    info.methodName = u"login"_s;
    info.anonymous = true;
    job.setMethodCallJobInfo(info);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"message":"{}"})").toLatin1());

    QVariantMap map;
    map.insert(u"msg"_s, u"method"_s);
    map.insert(u"id"_s, u"52"_s);
    map.insert(u"method"_s, u"login"_s);
    info.messageObj = QJsonObject::fromVariantMap(map);
    info.methodName = u"login"_s;
    info.anonymous = true;
    job.setMethodCallJobInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             u"{\"message\":\"{\\\"id\\\":\\\"52\\\",\\\"method\\\":\\\"login\\\",\\\"msg\\\":\\\"method\\\"}\"}"_s.toLatin1());
}

void MethodCallJobTest::shouldNotStarting()
{
    MethodCallJob job;

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
    // TODO
}

#include "moc_methodcalljobtest.cpp"
