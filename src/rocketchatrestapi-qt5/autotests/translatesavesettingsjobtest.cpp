/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "translatesavesettingsjobtest.h"
#include "autotranslate/translatesavesettingsjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
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
    QCOMPARE(job.type(), TranslateSaveSettingsJob::Underfined);
    QVERIFY(!job.autoTranslate());
    QVERIFY(!job.hasQueryParameterSupport());
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

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"field\":\"%1\",\"roomId\":\"%2\",\"value\":true}").arg(QStringLiteral("autoTranslate")).arg(roomId).toLatin1());
    type = TranslateSaveSettingsJob::LanguageSetting;
    job.setType(type);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"field\":\"%1\",\"roomId\":\"%2\",\"value\":\"%3\"}").arg(QStringLiteral("autoTranslateLanguage")).arg(roomId).arg(targetLanguage).toLatin1());
}

void TranslateSaveSettingsJobTest::shouldNotStarting()
{
    TranslateSaveSettingsJob job;

    RestApiMethod *method = new RestApiMethod;
    method->setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(method);

    QNetworkAccessManager *mNetworkAccessManager = new QNetworkAccessManager;
    job.setNetworkAccessManager(mNetworkAccessManager);
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
    delete method;
    delete mNetworkAccessManager;
}
