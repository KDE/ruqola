/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deleteownaccountjobtest.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

#include "users/deleteownaccountjob.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(DeleteOwnAccountJobTest)
using namespace RocketChatRestApi;
DeleteOwnAccountJobTest::DeleteOwnAccountJobTest(QObject *parent)
    : QObject(parent)
{
}

void DeleteOwnAccountJobTest::shouldHaveDefaultValue()
{
    DeleteOwnAccountJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.password().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void DeleteOwnAccountJobTest::shouldGenerateRequest()
{
    DeleteOwnAccountJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.deleteOwnAccount"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void DeleteOwnAccountJobTest::shouldGenerateJson()
{
    DeleteOwnAccountJob job;
    const QString password = u"foo1"_s;
    job.setPassword(password);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"password":"bb4eca334f61af3b67b5d528907d30285151610200539302f4c8cabe66225b53"})").toLatin1());
}

void DeleteOwnAccountJobTest::shouldNotStarting()
{
    DeleteOwnAccountJob job;

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
    const QString password = u"foo1"_s;
    job.setPassword(password);
    QVERIFY(job.canStart());
}

#include "moc_deleteownaccountjobtest.cpp"
