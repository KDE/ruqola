/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatesavesettingsjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "autotranslate/translatesavesettingsjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(TranslateSaveSettingsJobTest)
using namespace RocketChatRestApi;
TranslateSaveSettingsJobTest::TranslateSaveSettingsJobTest(QObject *parent)
    : QObject(parent)
{
}

void TranslateSaveSettingsJobTest::shouldHaveDefaultValue()
{
    TranslateSaveSettingsJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.language().isEmpty());
    QCOMPARE(job.type(), TranslateSaveSettingsJob::SettingType::Undefined);
    QVERIFY(!job.autoTranslate());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void TranslateSaveSettingsJobTest::shouldGenerateRequest()
{
    TranslateSaveSettingsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/autotranslate.saveSettings"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void TranslateSaveSettingsJobTest::shouldGenerateJson()
{
    TranslateSaveSettingsJob job;
    const QString roomId = u"foo1"_s;
    job.setRoomId(roomId);
    const QString targetLanguage = u"bla"_s;
    job.setLanguage(targetLanguage);
    TranslateSaveSettingsJob::SettingType type = TranslateSaveSettingsJob::SettingType::AutoTranslateSetting;
    job.setType(type);
    const bool autoTranslate = true;
    job.setAutoTranslate(autoTranslate);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), u"{\"field\":\"%1\",\"roomId\":\"%2\",\"value\":true}"_s.arg(u"autoTranslate"_s, roomId).toLatin1());
    type = TranslateSaveSettingsJob::SettingType::LanguageSetting;
    job.setType(type);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             u"{\"field\":\"%1\",\"roomId\":\"%2\",\"value\":\"%3\"}"_s.arg(u"autoTranslateLanguage"_s, roomId, targetLanguage).toLatin1());
}

void TranslateSaveSettingsJobTest::shouldNotStarting()
{
    TranslateSaveSettingsJob job;

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
    QVERIFY(!job.canStart());
    TranslateSaveSettingsJob::SettingType type = TranslateSaveSettingsJob::SettingType::AutoTranslateSetting;
    job.setType(type);

    const QString roomId = u"foo1"_s;
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
    type = TranslateSaveSettingsJob::SettingType::LanguageSetting;
    job.setType(type);
    QVERIFY(!job.canStart());
    const QString targetLanguage = u"bla"_s;
    job.setLanguage(targetLanguage);
    QVERIFY(job.canStart());
}

#include "moc_translatesavesettingsjobtest.cpp"
