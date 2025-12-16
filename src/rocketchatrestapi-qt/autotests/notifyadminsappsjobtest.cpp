/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notifyadminsappsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "apps/notifyadminsappsjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(NotifyAdminsAppsJobTest)
using namespace RocketChatRestApi;
NotifyAdminsAppsJobTest::NotifyAdminsAppsJobTest(QObject *parent)
    : QObject(parent)
{
}

void NotifyAdminsAppsJobTest::shouldHaveDefaultValue()
{
    NotifyAdminsAppsJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.info().isValid());
}

void NotifyAdminsAppsJobTest::shouldGenerateRequest()
{
    NotifyAdminsAppsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/apps/notify-admins"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void NotifyAdminsAppsJobTest::shouldGenerateJson()
{
    NotifyAdminsAppsJob job;
    const NotifyAdminsAppsJob::NotifyAdminsAppsInfo info{"app1 id"_ba, u"app test"_s, u"app version"_s, u"test message"_s};
    job.setInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"appId":"app1 id","appName":"app test","appVersion":"app version","message":"test message"})").toLatin1());
}

void NotifyAdminsAppsJobTest::shouldNotStarting()
{
    NotifyAdminsAppsJob job;

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
    const NotifyAdminsAppsJob::NotifyAdminsAppsInfo info{"app1 id"_ba, u"app test"_s, u"app version"_s, u"test message"_s};
    job.setInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_notifyadminsappsjobtest.cpp"
