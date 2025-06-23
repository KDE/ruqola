/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "spotlightjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "spotlightjob.h"
#include <QTest>
QTEST_GUILESS_MAIN(SpotlightJobTest)
using namespace RocketChatRestApi;
SpotlightJobTest::SpotlightJobTest(QObject *parent)
    : QObject(parent)
{
}

void SpotlightJobTest::shouldHaveDefaultValue()
{
    SpotlightJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.searchPattern().isEmpty());
    QVERIFY(!job.restApiLogger());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void SpotlightJobTest::shouldGenerateRequest()
{
    SpotlightJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QString avatarUserId = u"avat"_s;
    job.setSearchPattern(avatarUserId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/spotlight?query=avat"_s));
}

#include "moc_spotlightjobtest.cpp"
