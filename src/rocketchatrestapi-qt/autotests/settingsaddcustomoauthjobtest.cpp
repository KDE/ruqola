/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "settingsaddcustomoauthjobtest.h"

#include "ruqola_restapi_helper.h"

#include "settings/settingsaddcustomoauthjob.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(SettingsAddCustomOAuthJobTest)
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
SettingsAddCustomOAuthJobTest::SettingsAddCustomOAuthJobTest(QObject *parent)
    : QObject(parent)
{
}

void SettingsAddCustomOAuthJobTest::shouldHaveDefaultValue()
{
    SettingsAddCustomOAuthJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.requireTwoFactorAuthentication());
    QVERIFY(job.name().isEmpty());
}

void SettingsAddCustomOAuthJobTest::shouldGenerateRequest()
{
    SettingsAddCustomOAuthJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/settings.addCustomOAuth"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void SettingsAddCustomOAuthJobTest::shouldGenerateJson()
{
    SettingsAddCustomOAuthJob job;
    const QString name = u"foo"_s;
    job.setName(name);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), u"{\"name\":\"%1\"}"_s.arg(name).toLatin1());
}

void SettingsAddCustomOAuthJobTest::shouldNotStarting()
{
    SettingsAddCustomOAuthJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    QVERIFY(!job.canStart());

    job.setAuthCode(u"bla"_s);
    QVERIFY(!job.canStart());

    job.setAuthMethod(u"method"_s);
    QVERIFY(!job.canStart());

    job.setName(u"foo"_s);
    QVERIFY(job.canStart());
}

#include "moc_settingsaddcustomoauthjobtest.cpp"
