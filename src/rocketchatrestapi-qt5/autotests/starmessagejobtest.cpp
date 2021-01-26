/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#include "starmessagejobtest.h"
#include "chat/starmessagejob.h"
#include <QJsonDocument>
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(StarMessageJobTest)
using namespace RocketChatRestApi;
StarMessageJobTest::StarMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void StarMessageJobTest::shouldHaveDefaultValue()
{
    StarMessageJob job;
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.start());
    QVERIFY(job.authToken().isEmpty());
    QVERIFY(job.userId().isEmpty());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.starMessage());
    QVERIFY(!job.hasQueryParameterSupport());
}

void StarMessageJobTest::shouldHaveMessageId()
{
    StarMessageJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    QVERIFY(!job.canStart());
    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    job.setMessageId(QStringLiteral("bla"));
    QVERIFY(job.canStart());
}

void StarMessageJobTest::shouldGenerateStarMessageRequest()
{
    StarMessageJob job;
    RestApiMethod method;
    const QString authToken = QStringLiteral("foo");
    const QString userId = QStringLiteral("user");
    job.setUserId(userId);
    job.setAuthToken(authToken);
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QString messageId = QStringLiteral("foo");
    job.setMessageId(messageId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.starMessage")));
    QCOMPARE(request.attribute(QNetworkRequest::HttpPipeliningAllowedAttribute).toBool(), true);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QCOMPARE(request.attribute(QNetworkRequest::HTTP2AllowedAttribute).toBool(), true);
#else
    QCOMPARE(request.attribute(QNetworkRequest::Http2AllowedAttribute).toBool(), true);
#endif
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
    QCOMPARE(request.rawHeader(QByteArrayLiteral("X-Auth-Token")), authToken.toLocal8Bit());
    QCOMPARE(request.rawHeader(QByteArrayLiteral("X-User-Id")), userId.toLocal8Bit());
}

void StarMessageJobTest::shouldGenerateUnStarMessageRequest()
{
    StarMessageJob job;
    job.setStarMessage(false);
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    const QString messageId = QStringLiteral("foo");
    job.setMessageId(messageId);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.unStarMessage")));
    QCOMPARE(request.attribute(QNetworkRequest::HttpPipeliningAllowedAttribute).toBool(), true);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QCOMPARE(request.attribute(QNetworkRequest::HTTP2AllowedAttribute).toBool(), true);
#else
    QCOMPARE(request.attribute(QNetworkRequest::Http2AllowedAttribute).toBool(), true);
#endif
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void StarMessageJobTest::shouldGenerateJson()
{
    StarMessageJob job;
    const QString messageId = QStringLiteral("foo1");
    job.setMessageId(messageId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"messageId":"%1"})").arg(messageId).toLatin1());
}
