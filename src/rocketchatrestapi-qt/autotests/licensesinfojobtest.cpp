/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "licensesinfojobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "license/licensesinfojob.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(LicensesInfoJobTest)
using namespace RocketChatRestApi;
LicensesInfoJobTest::LicensesInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void LicensesInfoJobTest::shouldHaveDefaultValue()
{
    LicensesInfoJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void LicensesInfoJobTest::shouldGenerateRequest()
{
    LicensesInfoJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/licenses.info"_s));
    }
}

#include "moc_licensesinfojobtest.cpp"
