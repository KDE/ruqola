/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setroomkeyidjobtest.h"

#include "e2e/setroomkeyidjob.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(SetRoomKeyIDJobTest)
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
SetRoomKeyIDJobTest::SetRoomKeyIDJobTest(QObject *parent)
    : QObject(parent)
{
}

void SetRoomKeyIDJobTest::shouldHaveDefaultValue()
{
    SetRoomKeyIDJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());

    QVERIFY(!job.roomKeyIDInfo().isValid());
    QVERIFY(!job.hasQueryParameterSupport());
}

void SetRoomKeyIDJobTest::shouldGenerateRequest()
{
    SetRoomKeyIDJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/e2e.setRoomKeyID"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void SetRoomKeyIDJobTest::shouldGenerateJson()
{
    SetRoomKeyIDJob job;
    SetRoomKeyIDJob::RoomKeyIDInfo info;
    const QByteArray roomId = "foo1"_ba;
    const QByteArray keyId = "key1"_ba;
    info.roomId = roomId;
    info.keyId = keyId;
    job.setRoomKeyIDInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             uR"({"keyID":"%2","rid":"%1"})"_s.arg(QString::fromLatin1(roomId), QString::fromLatin1(keyId)).toLatin1());
}

void SetRoomKeyIDJobTest::shouldNotStarting()
{
    SetRoomKeyIDJob job;

    RuqolaRestApiHelper::verifyNotStartingJob(&job);

    SetRoomKeyIDJob::RoomKeyIDInfo info;
    const QByteArray roomId = "foo1"_ba;
    const QByteArray keyId = "key1"_ba;
    info.roomId = roomId;
    job.setRoomKeyIDInfo(info);
    QVERIFY(!job.canStart());
    info.keyId = keyId;
    job.setRoomKeyIDInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_setroomkeyidjobtest.cpp"
