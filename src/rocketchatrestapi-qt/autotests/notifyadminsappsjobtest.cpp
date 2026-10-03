/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notifyadminsappsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "apps/notifyadminsappsjob.h"
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
             uR"({"appId":"app1 id","appName":"app test","appVersion":"app version","message":"test message"})"_s.toLatin1());
}

void NotifyAdminsAppsJobTest::shouldNotStarting()
{
    NotifyAdminsAppsJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const NotifyAdminsAppsJob::NotifyAdminsAppsInfo info{"app1 id"_ba, u"app test"_s, u"app version"_s, u"test message"_s};
    job.setInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_notifyadminsappsjobtest.cpp"
