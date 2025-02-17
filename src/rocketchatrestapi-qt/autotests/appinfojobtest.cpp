/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appinfojobtest.h"
#include "apps/appinfojob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(AppInfoJobTest)
using namespace RocketChatRestApi;
AppInfoJobTest::AppInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void AppInfoJobTest::shouldHaveDefaultValue()
{
    AppInfoJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void AppInfoJobTest::shouldGenerateRequest()
{
    {
        AppInfoJob job;
        const QByteArray ba{"foo-bla-bli"_ba};
        job.setAppsId(ba);
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/apps/%1/").arg(QString::fromLatin1(ba))));
    }
    {
        AppInfoJob job;
        const QByteArray ba{"foo-bla-bli"_ba};
        job.setAppsId(ba);
        job.setAppInfoType(AppInfoJob::AppInfoType::Logs);
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/apps/%1/logs").arg(QString::fromLatin1(ba))));
    }
    {
        AppInfoJob job;
        const QByteArray ba{"foo-bla-bli"_ba};
        job.setAppsId(ba);
        job.setAppInfoType(AppInfoJob::AppInfoType::Versions);
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/apps/%1/versions").arg(QString::fromLatin1(ba))));
    }
    {
        AppInfoJob job;
        const QByteArray ba{"foo-bla-bli"_ba};
        job.setAppsId(ba);
        job.setAppInfoType(AppInfoJob::AppInfoType::ScreenShots);
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/apps/%1/screenshots").arg(QString::fromLatin1(ba))));
    }
    {
        AppInfoJob job;
        const QByteArray ba{"foo-bla-bli"_ba};
        job.setAppsId(ba);
        job.setAppInfoType(AppInfoJob::AppInfoType::Settings);
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/apps/%1/settings").arg(QString::fromLatin1(ba))));
    }
}

#include "moc_appinfojobtest.cpp"
