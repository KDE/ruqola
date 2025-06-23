/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appcategoriesjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "apps/appcategoriesjob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(AppCategoriesJobTest)
using namespace RocketChatRestApi;
AppCategoriesJobTest::AppCategoriesJobTest(QObject *parent)
    : QObject(parent)
{
}

void AppCategoriesJobTest::shouldHaveDefaultValue()
{
    AppCategoriesJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void AppCategoriesJobTest::shouldGenerateRequest()
{
    AppCategoriesJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/apps/categories"_s));
}

#include "moc_appcategoriesjobtest.cpp"
