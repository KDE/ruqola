/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportuserjobtest.h"
#include "moderation/moderationreportuserjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ModerationReportUserJobTest)
using namespace RocketChatRestApi;
ModerationReportUserJobTest::ModerationReportUserJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationReportUserJobTest::shouldHaveDefaultValue()
{
    ModerationReportUserJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.reportedUserId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.description().isEmpty());
}

void ModerationReportUserJobTest::shouldGenerateRequest()
{
    ModerationReportUserJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/moderation.reportUser")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ModerationReportUserJobTest::shouldGenerateJson()
{
    {
        ModerationReportUserJob job;
        const QByteArray reportedUserId("foo2");
        job.setReportedUserId(reportedUserId);

        QCOMPARE(job.json().toJson(QJsonDocument::Compact),
                 QStringLiteral(R"({"description":"","userId":"%1"})").arg(QLatin1StringView(reportedUserId)).toLatin1());
    }
    {
        ModerationReportUserJob job;
        const QByteArray reportedUserId("foo7");
        job.setReportedUserId(reportedUserId);
        const QString description(QStringLiteral("test"));
        job.setDescription(description);

        QCOMPARE(job.json().toJson(QJsonDocument::Compact),
                 QStringLiteral(R"({"description":"%1","userId":"%2"})").arg(description, QLatin1StringView(reportedUserId)).toLatin1());
    }
}

void ModerationReportUserJobTest::shouldNotStarting()
{
    ModerationReportUserJob job;

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
    const QByteArray reportedUserId("foo2");
    job.setReportedUserId(reportedUserId);
    QVERIFY(!job.canStart());
    const QString description(QStringLiteral("test"));
    job.setDescription(description);
    QVERIFY(job.canStart());
}

#include "moc_moderationreportuserjobtest.cpp"
