/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannersdismissjobtest.h"
#include "banner/bannersdismissjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(BannersDismissJobTest)
using namespace RocketChatRestApi;
BannersDismissJobTest::BannersDismissJobTest(QObject *parent)
    : QObject(parent)
{
}

void BannersDismissJobTest::shouldHaveDefaultValue()
{
    BannersDismissJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.bannerId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void BannersDismissJobTest::shouldGenerateRequest()
{
    BannersDismissJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/banners.dismiss")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void BannersDismissJobTest::shouldGenerateJson()
{
    BannersDismissJob job;
    const QString bannerId = QStringLiteral("foo1");
    job.setBannerId(bannerId);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"bannerId":"%1"})").arg(bannerId).toLatin1());
}

void BannersDismissJobTest::shouldNotStarting()
{
    BannersDismissJob job;

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
    const QString bannerId = QStringLiteral("foo1");
    job.setBannerId(bannerId);
    QVERIFY(job.canStart());
}

#include "moc_bannersdismissjobtest.cpp"
