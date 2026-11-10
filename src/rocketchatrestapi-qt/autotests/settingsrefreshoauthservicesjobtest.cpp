/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "settingsrefreshoauthservicesjobtest.h"

#include "ruqola_restapi_helper.h"

#include "settings/settingsrefreshoauthservicesjob.h"
#include <QTest>

QTEST_GUILESS_MAIN(SettingsRefreshOAuthServicesJobTest)
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
SettingsRefreshOAuthServicesJobTest::SettingsRefreshOAuthServicesJobTest(QObject *parent)
    : QObject(parent)
{
}

void SettingsRefreshOAuthServicesJobTest::shouldHaveDefaultValue()
{
    SettingsRefreshOAuthServicesJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.requireTwoFactorAuthentication());
}

void SettingsRefreshOAuthServicesJobTest::shouldGenerateRequest()
{
    SettingsRefreshOAuthServicesJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/settings.refreshOAuthServices"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void SettingsRefreshOAuthServicesJobTest::shouldNotStarting()
{
    SettingsRefreshOAuthServicesJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    QVERIFY(!job.canStart());

    job.setAuthCode(u"bla"_s);
    QVERIFY(!job.canStart());

    job.setAuthMethod(u"method"_s);
    QVERIFY(job.canStart());
}

#include "moc_settingsrefreshoauthservicesjobtest.cpp"
