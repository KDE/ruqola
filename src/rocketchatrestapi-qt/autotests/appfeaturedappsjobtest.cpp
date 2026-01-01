/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appfeaturedappsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "apps/appfeaturedappsjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(AppFeaturedAppsJobTest)
using namespace RocketChatRestApi;
AppFeaturedAppsJobTest::AppFeaturedAppsJobTest(QObject *parent)
    : QObject(parent)
{
}

void AppFeaturedAppsJobTest::shouldHaveDefaultValue()
{
    AppFeaturedAppsJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void AppFeaturedAppsJobTest::shouldGenerateRequest()
{
    AppFeaturedAppsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/apps/featured-apps"_s));
}

#include "moc_appfeaturedappsjobtest.cpp"
