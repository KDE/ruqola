/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatesavesettingsjobtest.h"
#include "autotranslate/translatesavesettingsjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(TranslateSaveSettingsJobTest)
using namespace RocketChatRestApi;
TranslateSaveSettingsJobTest::TranslateSaveSettingsJobTest(QObject *parent)
    : QObject(parent)
{
}

void TranslateSaveSettingsJobTest::shouldHaveDefaultValue()
{
    TranslateSaveSettingsJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.language().isEmpty());
    QCOMPARE(job.type(), TranslateSaveSettingsJob::Undefined);
    QVERIFY(!job.autoTranslate());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void TranslateSaveSettingsJobTest::shouldGenerateRequest()
{
    TranslateSaveSettingsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/autotranslate.saveSettings")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void TranslateSaveSettingsJobTest::shouldGenerateJson()
{
    TranslateSaveSettingsJob job;
    const QString roomId = QStringLiteral("foo1");
    job.setRoomId(roomId);
    const QString targetLanguage = QStringLiteral("bla");
    job.setLanguage(targetLanguage);
    TranslateSaveSettingsJob::SettingType type = TranslateSaveSettingsJob::AutoTranslateSetting;
    job.setType(type);
    bool autoTranslate = true;
    job.setAutoTranslate(autoTranslate);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral("{\"field\":\"%1\",\"roomId\":\"%2\",\"value\":true}").arg(QStringLiteral("autoTranslate"), roomId).toLatin1());
    type = TranslateSaveSettingsJob::LanguageSetting;
    job.setType(type);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral("{\"field\":\"%1\",\"roomId\":\"%2\",\"value\":\"%3\"}")
                 .arg(QStringLiteral("autoTranslateLanguage"), roomId, targetLanguage)
                 .toLatin1());
}

void TranslateSaveSettingsJobTest::shouldNotStarting()
{
    TranslateSaveSettingsJob job;

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
    QVERIFY(!job.canStart());
    TranslateSaveSettingsJob::SettingType type = TranslateSaveSettingsJob::AutoTranslateSetting;
    job.setType(type);

    const QString roomId = QStringLiteral("foo1");
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
    type = TranslateSaveSettingsJob::LanguageSetting;
    job.setType(type);
    QVERIFY(!job.canStart());
    const QString targetLanguage = QStringLiteral("bla");
    job.setLanguage(targetLanguage);
    QVERIFY(job.canStart());
}

#include "moc_translatesavesettingsjobtest.cpp"
