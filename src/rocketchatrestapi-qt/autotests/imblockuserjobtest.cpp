/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "imblockuserjobtest.h"

#include "im/imblockuserjob.h"
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
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"block":false,"roomId":"%1"})"_s.arg(QLatin1StringView(roomId)).toLatin1());

    job.setBlockUser(true);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"block":true,"roomId":"%1"})"_s.arg(QLatin1StringView(roomId)).toLatin1());
}

void ImBlockUserJobTest::shouldNotStarting()
{
    ImBlockUserJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);

    const QByteArray roomId("foo1");
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_imblockuserjobtest.cpp"
