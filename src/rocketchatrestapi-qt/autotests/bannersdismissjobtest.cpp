/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannersdismissjobtest.h"
#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

#include "banner/bannersdismissjob.h"
#include "restapimethod.h"
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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.bannerId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void BannersDismissJobTest::shouldGenerateRequest()
{
    BannersDismissJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/banners.dismiss"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void BannersDismissJobTest::shouldGenerateJson()
{
    BannersDismissJob job;
    const QString bannerId = u"foo1"_s;
    job.setBannerId(bannerId);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"bannerId":"%1"})").arg(bannerId).toLatin1());
}

void BannersDismissJobTest::shouldNotStarting()
{
    BannersDismissJob job;

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
    const QString bannerId = u"foo1"_s;
    job.setBannerId(bannerId);
    QVERIFY(job.canStart());
}

#include "moc_bannersdismissjobtest.cpp"
