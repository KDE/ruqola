/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "updateadminsettingsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_restapi_helper.h"
#include "settings/updateadminsettingsjob.h"
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
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/settings"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
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
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    UpdateAdminSettingsJob::UpdateAdminSettingsInfo info;
    info.settingName = u"bla"_s;
    info.settingsValue = false;
    info.valueType = UpdateAdminSettingsJob::UpdateAdminSettingsInfo::ValueType::Boolean;

    job.setUpdateAdminSettingsInfo(info);

    QVERIFY(!job.canStart());

    job.setAuthCode(u"bla"_s);
    QVERIFY(!job.canStart());

    job.setAuthMethod(u"method"_s);
    QVERIFY(job.canStart());
}

#include "moc_updateadminsettingsjobtest.cpp"
