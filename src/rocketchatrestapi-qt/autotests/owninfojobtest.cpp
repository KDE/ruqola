/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "owninfojobtest.h"
#include "misc/owninfojob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(OwnInfoJobTest)
using namespace RocketChatRestApi;
OwnInfoJobTest::OwnInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void OwnInfoJobTest::shouldHaveDefaultValue()
{
    OwnInfoJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void OwnInfoJobTest::shouldGenerateRequest()
{
    OwnInfoJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/me")));
}

#include "moc_owninfojobtest.cpp"
