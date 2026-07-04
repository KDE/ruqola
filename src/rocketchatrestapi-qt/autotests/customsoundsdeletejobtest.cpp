/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundsdeletejobtest.h"

#include "custom/customsoundsdeletejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(CustomSoundsDeleteJobTest)
using namespace RocketChatRestApi;
CustomSoundsDeleteJobTest::CustomSoundsDeleteJobTest(QObject *parent)
    : QObject(parent)
{
}

void CustomSoundsDeleteJobTest::shouldHaveDefaultValue()
{
    CustomSoundsDeleteJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void CustomSoundsDeleteJobTest::shouldGenerateRequest()
{
    CustomSoundsDeleteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/custom-sounds.delete"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void CustomSoundsDeleteJobTest::shouldGenerateJson()
{
    CustomSoundsDeleteJob job;

    const QByteArray customSoundId("foo1");
    job.setCustomSoundId(customSoundId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"_id":"%1"})").arg(QLatin1StringView(customSoundId)).toLatin1());
}

void CustomSoundsDeleteJobTest::shouldNotStarting()
{
    CustomSoundsDeleteJob job;

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

    const QByteArray customSoundId("foo1");
    job.setCustomSoundId(customSoundId);
    QVERIFY(job.canStart());
}

#include "moc_customsoundsdeletejobtest.cpp"
