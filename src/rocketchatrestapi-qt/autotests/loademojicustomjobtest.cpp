/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "loademojicustomjobtest.h"
#include "emoji/loademojicustomjob.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(LoadEmojiCustomJobTest)
using namespace RocketChatRestApi;
LoadEmojiCustomJobTest::LoadEmojiCustomJobTest(QObject *parent)
    : QObject(parent)
{
}

void LoadEmojiCustomJobTest::shouldHaveDefaultValue()
{
    LoadEmojiCustomJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.authToken().isEmpty());
    QVERIFY(job.authCode().isEmpty());
    QVERIFY(job.authMethod().isEmpty());
    QVERIFY(job.userId().isEmpty());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void LoadEmojiCustomJobTest::shouldGenerateRequest()
{
    LoadEmojiCustomJob job;
    const QString authToken = QStringLiteral("foo");
    const QString userId = QStringLiteral("user");
    job.setUserId(userId);
    job.setAuthToken(authToken);
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/emoji-custom.list")));
    QCOMPARE(request.rawHeader(QByteArrayLiteral("X-Auth-Token")), authToken.toLocal8Bit());
    QCOMPARE(request.rawHeader(QByteArrayLiteral("X-User-Id")), userId.toLocal8Bit());
}

#include "moc_loademojicustomjobtest.cpp"
