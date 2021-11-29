/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "licenseslistjobtest.h"
#include "license/licenseslistjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
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
