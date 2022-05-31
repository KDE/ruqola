/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "updateadminsettingsjobtest.h"
#include "ruqola_restapi_helper.h"
#include "settings/updateadminsettingsjob.h"
#include <QTest>
QTEST_GUILESS_MAIN(UpdateAdminSettingsJobTest)
using namespace RocketChatRestApi;
UpdateAdminSettingsJobTest::UpdateAdminSettingsJobTest(QObject *parent)
    : QObject(parent)
{
}

void UpdateAdminSettingsJobTest::shouldHaveDefaultValue()
{
    UpdateAdminSettingsJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.requireTwoFactorAuthentication());
}

void UpdateAdminSettingsJobTest::shouldGenerateRequest()
{
    UpdateAdminSettingsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/settings")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void UpdateAdminSettingsJobTest::shouldGenerateJson()
{
    UpdateAdminSettingsJob job;
    // TODO
}

void UpdateAdminSettingsJobTest::shouldNotStarting()
{
    UpdateAdminSettingsJob job;

    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    UpdateAdminSettingsJob::UpdateAdminSettingsInfo info;
    info.settingName = QStringLiteral("bla");
    info.settingsValue = false;
    info.valueType = UpdateAdminSettingsJob::UpdateAdminSettingsInfo::ValueType::Boolean;

    job.setUpdateAdminSettingsInfo(info);

    QVERIFY(!job.canStart());

    job.setAuthCode(QStringLiteral("bla"));
    QVERIFY(!job.canStart());

    job.setAuthMethod(QStringLiteral("method"));
    QVERIFY(job.canStart());
}
