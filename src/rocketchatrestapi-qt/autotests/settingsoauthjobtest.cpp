/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "settingsoauthjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/settingsoauthjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
QTEST_GUILESS_MAIN(SettingsOauthJobTest)
using namespace RocketChatRestApi;
SettingsOauthJobTest::SettingsOauthJobTest(QObject *parent)
    : QObject(parent)
{
}

void SettingsOauthJobTest::shouldHaveDefaultValue()
{
    SettingsOauthJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void SettingsOauthJobTest::shouldGenerateRequest()
{
    SettingsOauthJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/settings.oauth"_s));
}

#include "moc_settingsoauthjobtest.cpp"
