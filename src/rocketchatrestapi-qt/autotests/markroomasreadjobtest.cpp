/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "markroomasreadjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "ruqola_restapi_helper.h"

#include "subscriptions/markroomasreadjob.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(MarkRoomAsReadJobTest)
using namespace RocketChatRestApi;
MarkRoomAsReadJobTest::MarkRoomAsReadJobTest(QObject *parent)
    : QObject(parent)
{
}

void MarkRoomAsReadJobTest::shouldHaveDefaultValue()
{
    MarkRoomAsReadJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void MarkRoomAsReadJobTest::shouldGenerateRequest()
{
    MarkRoomAsReadJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/subscriptions.read"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void MarkRoomAsReadJobTest::shouldGenerateJson()
{
    MarkRoomAsReadJob job;
    const QByteArray roomId = "foo1"_ba;
    job.setRoomId(roomId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), uR"({"rid":"%1"})"_s.arg(QLatin1StringView(roomId)).toLatin1());
}

void MarkRoomAsReadJobTest::shouldNotStarting()
{
    MarkRoomAsReadJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);
    const QByteArray roomId = "foo1"_ba;
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

#include "moc_markroomasreadjobtest.cpp"
