/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthappscreatejobtest.h"
#include "misc/oauthappscreatejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(OauthAppsCreateJobTest)
using namespace RocketChatRestApi;
OauthAppsCreateJobTest::OauthAppsCreateJobTest(QObject *parent)
    : QObject(parent)
{
}

void OauthAppsCreateJobTest::shouldHaveDefaultValue()
{
    OauthAppsCreateJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    // QVERIFY(job.permissions().isEmpty());
}

void OauthAppsCreateJobTest::shouldGenerateRequest()
{
    OauthAppsCreateJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/oauth-apps.create")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void OauthAppsCreateJobTest::shouldGenerateJson()
{
    OauthAppsCreateJob job;
    //    QMap<QString, QStringList> lst;
    //    lst.insert(QStringLiteral("bla"), {QStringLiteral("user"), QStringLiteral("admin")});
    //    lst.insert(QStringLiteral("team"), {QStringLiteral("user"), QStringLiteral("admin"), QStringLiteral("owner")});
    //    job.setPermissions(lst);

    //    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
    //             QStringLiteral(R"({"permissions":[{"_id":"bla","roles":["user","admin"]},{"_id":"team","roles":["user","admin","owner"]}]})").toLatin1());
}

void OauthAppsCreateJobTest::shouldNotStarting()
{
    OauthAppsCreateJob job;

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
    QVERIFY(job.canStart());
}
