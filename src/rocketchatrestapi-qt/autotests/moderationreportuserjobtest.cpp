/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportuserjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "moderation/moderationreportuserjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(ModerationReportUserJobTest)
using namespace RocketChatRestApi;
ModerationReportUserJobTest::ModerationReportUserJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationReportUserJobTest::shouldHaveDefaultValue()
{
    ModerationReportUserJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.reportedUserId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.description().isEmpty());
}

void ModerationReportUserJobTest::shouldGenerateRequest()
{
    ModerationReportUserJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/moderation.reportUser"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
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
        const QString description(u"test"_s);
        job.setDescription(description);

        QCOMPARE(job.json().toJson(QJsonDocument::Compact),
                 QStringLiteral(R"({"description":"%1","userId":"%2"})").arg(description, QLatin1StringView(reportedUserId)).toLatin1());
    }
}

void ModerationReportUserJobTest::shouldNotStarting()
{
    ModerationReportUserJob job;

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
    const QByteArray reportedUserId("foo2");
    job.setReportedUserId(reportedUserId);
    QVERIFY(!job.canStart());
    const QString description(u"test"_s);
    job.setDescription(description);
    QVERIFY(job.canStart());
}

#include "moc_moderationreportuserjobtest.cpp"
