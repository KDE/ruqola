/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojicustomalljobtest.h"
#include "emoji/emojicustomalljob.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(EmojiCustomAllJobTest)
using namespace RocketChatRestApi;
EmojiCustomAllJobTest::EmojiCustomAllJobTest(QObject *parent)
    : QObject(parent)
{
}

void EmojiCustomAllJobTest::shouldHaveDefaultValue()
{
    EmojiCustomAllJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.authToken().isEmpty());
    QVERIFY(job.userId().isEmpty());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.hasQueryParameterSupport());
}

void EmojiCustomAllJobTest::shouldGenerateRequest()
{
    {
        EmojiCustomAllJob job;
        const QString authToken = QStringLiteral("foo");
        const QString userId = QStringLiteral("user");
        job.setUserId(userId);
        job.setAuthToken(authToken);
        RestApiMethod method;
        method.setServerUrl(QStringLiteral("http://www.kde.org"));
        job.setRestApiMethod(&method);
        const QNetworkRequest request = job.request();
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/emoji-custom.all")));
        QCOMPARE(request.rawHeader(QByteArrayLiteral("X-Auth-Token")), authToken.toLocal8Bit());
        QCOMPARE(request.rawHeader(QByteArrayLiteral("X-User-Id")), userId.toLocal8Bit());
    }
    {
        EmojiCustomAllJob job;
        const QString authToken = QStringLiteral("foo");
        const QString userId = QStringLiteral("user");
        job.setUserId(userId);
        job.setAuthToken(authToken);
        RestApiMethod method;
        method.setServerUrl(QStringLiteral("http://www.kde.org"));
        job.setRestApiMethod(&method);

        QMap<QString, RocketChatRestApi::QueryParameters::SortOrder> map;
        map.insert(QStringLiteral("name"), RocketChatRestApi::QueryParameters::SortOrder::Ascendant);
        RocketChatRestApi::QueryParameters parameters;
        parameters.setSorting(map);
        parameters.setOffset(1);
        parameters.setCount(3);
        parameters.setSearchString(QStringLiteral("bla"));

        job.setQueryParameters(parameters);
        const QNetworkRequest request = job.request();
        QCOMPARE(request.url().query(),
                 QStringLiteral("count=3&offset=1&query=%7B%22name%22:%7B%22$regex%22:%22bla%22,%22$options%22:%22i%22%7D%7D&sort=%7B%22name%22:1%7D"));
        QCOMPARE(request.rawHeader(QByteArrayLiteral("X-Auth-Token")), authToken.toLocal8Bit());
        QCOMPARE(request.rawHeader(QByteArrayLiteral("X-User-Id")), userId.toLocal8Bit());
    }
}
