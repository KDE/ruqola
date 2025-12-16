/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appinfojobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "apps/appinfojob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(AppInfoJobTest)
using namespace RocketChatRestApi;
AppInfoJobTest::AppInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void AppInfoJobTest::shouldHaveDefaultValue()
{
    AppInfoJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
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
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/apps/%1/"_s.arg(QString::fromLatin1(ba))));
    }
    {
        AppInfoJob job;
        const QByteArray ba{"foo-bla-bli"_ba};
        job.setAppsId(ba);
        job.setAppInfoType(AppInfoJob::AppInfoType::Logs);
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/apps/%1/logs"_s.arg(QString::fromLatin1(ba))));
    }
    {
        AppInfoJob job;
        const QByteArray ba{"foo-bla-bli"_ba};
        job.setAppsId(ba);
        job.setAppInfoType(AppInfoJob::AppInfoType::Versions);
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/apps/%1/versions"_s.arg(QString::fromLatin1(ba))));
    }
    {
        AppInfoJob job;
        const QByteArray ba{"foo-bla-bli"_ba};
        job.setAppsId(ba);
        job.setAppInfoType(AppInfoJob::AppInfoType::ScreenShots);
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/apps/%1/screenshots"_s.arg(QString::fromLatin1(ba))));
    }
    {
        AppInfoJob job;
        const QByteArray ba{"foo-bla-bli"_ba};
        job.setAppsId(ba);
        job.setAppInfoType(AppInfoJob::AppInfoType::Settings);
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/apps/%1/settings"_s.arg(QString::fromLatin1(ba))));
    }
}

#include "moc_appinfojobtest.cpp"
