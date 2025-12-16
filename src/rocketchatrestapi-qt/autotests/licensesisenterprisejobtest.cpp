/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "licensesisenterprisejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "license/licensesisenterprisejob.h"

#include "ruqola_restapi_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(LicensesIsEnterpriseJobTest)
using namespace RocketChatRestApi;
LicensesIsEnterpriseJobTest::LicensesIsEnterpriseJobTest(QObject *parent)
    : QObject(parent)
{
}

void LicensesIsEnterpriseJobTest::shouldHaveDefaultValue()
{
    LicensesIsEnterpriseJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void LicensesIsEnterpriseJobTest::shouldGenerateRequest()
{
    LicensesIsEnterpriseJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/licenses.isEnterprise"_s));
    }
}

#include "moc_licensesisenterprisejobtest.cpp"
