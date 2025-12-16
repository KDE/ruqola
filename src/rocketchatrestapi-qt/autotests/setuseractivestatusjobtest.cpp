/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setuseractivestatusjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include "users/setuseractivestatusjob.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(SetUserActiveStatusJobTest)
using namespace RocketChatRestApi;
SetUserActiveStatusJobTest::SetUserActiveStatusJobTest(QObject *parent)
    : QObject(parent)
{
}

void SetUserActiveStatusJobTest::shouldHaveDefaultValue()
{
    SetUserActiveStatusJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.activateUserId().isEmpty());
    QVERIFY(job.activate());
    QVERIFY(!job.hasQueryParameterSupport());
}

void SetUserActiveStatusJobTest::shouldGenerateRequest()
{
    SetUserActiveStatusJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/users.setActiveStatus"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void SetUserActiveStatusJobTest::shouldGenerateJson()
{
    SetUserActiveStatusJob job;
    const QByteArray userId("foo1");
    job.setActivateUserId(userId);
    job.setActivate(false);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"activeStatus":false,"userId":"%1"})").arg(QLatin1StringView(userId)).toLatin1());
    job.setActivate(true);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"activeStatus":true,"userId":"%1"})").arg(QLatin1StringView(userId)).toLatin1());
}

void SetUserActiveStatusJobTest::shouldNotStarting()
{
    SetUserActiveStatusJob job;

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
    const QByteArray statusUserid("foo1");
    job.setActivateUserId(statusUserid);
    QVERIFY(job.canStart());
}

#include "moc_setuseractivestatusjobtest.cpp"
