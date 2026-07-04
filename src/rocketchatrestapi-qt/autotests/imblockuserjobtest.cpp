/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "imblockuserjobtest.h"

#include "im/imblockuserjob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(ImBlockUserJobTest)
using namespace RocketChatRestApi;
ImBlockUserJobTest::ImBlockUserJobTest(QObject *parent)
    : QObject(parent)
{
}

void ImBlockUserJobTest::shouldHaveDefaultValue()
{
    ImBlockUserJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ImBlockUserJobTest::shouldGenerateRequest()
{
    ImBlockUserJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/im.blockUser"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ImBlockUserJobTest::shouldGenerateJson()
{
    ImBlockUserJob job;

    const QByteArray roomId("foo1");
    job.setRoomId(roomId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"block":false,"roomId":"%1"})").arg(QLatin1StringView(roomId)).toLatin1());

    job.setBlockUser(true);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"block":true,"roomId":"%1"})").arg(QLatin1StringView(roomId)).toLatin1());
}

void ImBlockUserJobTest::shouldNotStarting()
{
    ImBlockUserJob job;

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

    const QByteArray roomId("foo1");
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_imblockuserjobtest.cpp"
