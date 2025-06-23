/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "loginjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "authentication/loginjob.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(LoginJobTest)

using namespace RocketChatRestApi;
LoginJobTest::LoginJobTest(QObject *parent)
    : QObject(parent)
{
}

void LoginJobTest::shouldHaveDefaultValue()
{
    LoginJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(job.authToken().isEmpty());
    QVERIFY(job.authCode().isEmpty());
    QVERIFY(job.authMethod().isEmpty());
    QVERIFY(job.userId().isEmpty());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
}

void LoginJobTest::shouldHaveArguments()
{
    {
        LoginJob job;
        RestApiMethod method;
        method.setServerUrl(u"http://www.kde.org"_s);
        job.setRestApiMethod(&method);
        QVERIFY(!job.canStart());
        QNetworkAccessManager mNetworkAccessManager;
        job.setNetworkAccessManager(&mNetworkAccessManager);
        QVERIFY(!job.canStart());
        const QString auth = u"foo"_s;
        const QString userId = u"foo"_s;
        job.setAuthToken(auth);
        QVERIFY(!job.canStart());
        job.setUserId(userId);
        QVERIFY(!job.canStart());
        job.setPassword(u"bla"_s);
        QVERIFY(!job.canStart());
        job.setUserName(u"foo"_s);
        QVERIFY(job.canStart());
    }
    {
        LoginJob job;
        RestApiMethod method;
        method.setServerUrl(u"http://www.kde.org"_s);
        job.setRestApiMethod(&method);
        QVERIFY(!job.canStart());
        QNetworkAccessManager mNetworkAccessManager;
        job.setNetworkAccessManager(&mNetworkAccessManager);
        QVERIFY(!job.canStart());
        const QString auth = u"foo"_s;
        const QString userId = u"foo"_s;
        job.setAuthToken(auth);
        QVERIFY(!job.canStart());
        job.setUserId(userId);
        QVERIFY(!job.canStart());
        job.setResume(u"bla"_s);
        QVERIFY(job.canStart());
    }
}

void LoginJobTest::shouldGenerateLoginRequest()
{
    LoginJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/login"_s));
}

void LoginJobTest::shouldGenerateJson()
{
    {
        LoginJob job;
        const QString password(u"bla"_s);
        const QString username(u"foo"_s);
        job.setPassword(password);
        job.setUserName(username);
        QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"password":"%1","user":"%2"})").arg(password, username).toLatin1());
        const QString code(u"1213"_s);
        job.setCode(code);
        QCOMPARE(job.json().toJson(QJsonDocument::Compact),
                 QStringLiteral(R"({"code":"%3","password":"%1","user":"%2"})").arg(password, username, code).toLatin1());
    }
    {
        LoginJob job;
        const QString resume(u"bla"_s);
        job.setResume(resume);
        QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"resume":"%1"})").arg(resume).toLatin1());
    }
}

#include "moc_loginjobtest.cpp"
