/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appupdateinfojobtest.h"
#include "apps/appupdateinfojob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(AppUpdateInfoJobTest)
using namespace RocketChatRestApi;
AppUpdateInfoJobTest::AppUpdateInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void AppUpdateInfoJobTest::shouldHaveDefaultValue()
{
    AppUpdateInfoJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void AppUpdateInfoJobTest::shouldGenerateRequest()
{
    {
        AppUpdateInfoJob job;

        RocketChatRestApi::AppUpdateInfoJob::AppUpdateInfo info;
        info.mAppInfoType = RocketChatRestApi::AppUpdateInfoJob::AppInfoType::Apps;
        info.mAppMode = RocketChatRestApi::AppUpdateInfoJob::AppMode::Delete;
        info.mAppsId = "foo"_ba;
        job.setAppUpdateInfo(info);

        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/apps/foo")));
    }

    {
        AppUpdateInfoJob job;
        RocketChatRestApi::AppUpdateInfoJob::AppUpdateInfo info;
        info.mAppInfoType = RocketChatRestApi::AppUpdateInfoJob::AppInfoType::Settings;
        info.mAppMode = RocketChatRestApi::AppUpdateInfoJob::AppMode::Post;
        info.mAppsId = "foo"_ba;
        job.setAppUpdateInfo(info);

        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/apps/foo/settings")));
    }

    {
        AppUpdateInfoJob job;
        RocketChatRestApi::AppUpdateInfoJob::AppUpdateInfo info;
        info.mAppInfoType = RocketChatRestApi::AppUpdateInfoJob::AppInfoType::Icon;
        info.mAppMode = RocketChatRestApi::AppUpdateInfoJob::AppMode::Get;
        info.mAppsId = "foo"_ba;
        job.setAppUpdateInfo(info);

        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/apps/foo/icon")));
    }

    {
        AppUpdateInfoJob job;
        RocketChatRestApi::AppUpdateInfoJob::AppUpdateInfo info;
        info.mAppInfoType = RocketChatRestApi::AppUpdateInfoJob::AppInfoType::ActionButton;
        info.mAppMode = RocketChatRestApi::AppUpdateInfoJob::AppMode::Get;
        info.mAppsId = "foo"_ba;
        job.setAppUpdateInfo(info);

        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/apps/actionButtons")));
    }
}

#include "moc_appupdateinfojobtest.cpp"
