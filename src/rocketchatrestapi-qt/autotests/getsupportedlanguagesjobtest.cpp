/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getsupportedlanguagesjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "autotranslate/getsupportedlanguagesjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(GetSupportedLanguagesJobTest)
using namespace RocketChatRestApi;
GetSupportedLanguagesJobTest::GetSupportedLanguagesJobTest(QObject *parent)
    : QObject(parent)
{
}

void GetSupportedLanguagesJobTest::shouldHaveDefaultValue()
{
    GetSupportedLanguagesJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.needTargetLanguage());
}

void GetSupportedLanguagesJobTest::shouldGenerateRequest()
{
    GetSupportedLanguagesJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/autotranslate.getSupportedLanguages"_s));
}

#include "moc_getsupportedlanguagesjobtest.cpp"
