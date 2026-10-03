/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannersdismissjobtest.h"
using namespace Qt::Literals::StringLiterals;

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

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"bannerId":"%1"})"_s.arg(bannerId).toLatin1());
}

void BannersDismissJobTest::shouldNotStarting()
{
    BannersDismissJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QString bannerId = u"foo1"_s;
    job.setBannerId(bannerId);
    QVERIFY(job.canStart());
}

#include "moc_bannersdismissjobtest.cpp"
