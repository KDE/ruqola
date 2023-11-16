/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "licenseslistjobtest.h"
#include "license/licenseslistjob.h"
#include "ruqola_restapi_helper.h"
#include <restapimethod.h>
QTEST_GUILESS_MAIN(LicensesListJobTest)
using namespace RocketChatRestApi;
LicensesListJobTest::LicensesListJobTest(QObject *parent)
    : QObject(parent)
{
}

void LicensesListJobTest::shouldHaveDefaultValue()
{
    LicensesListJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void LicensesListJobTest::shouldGenerateRequest()
{
    LicensesListJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/licenses.get")));
    }
}

#include "moc_licenseslistjobtest.cpp"
