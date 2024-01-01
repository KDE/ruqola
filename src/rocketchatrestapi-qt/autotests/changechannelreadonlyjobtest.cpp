/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changechannelreadonlyjobtest.h"
#include "channels/changechannelreadonlyjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ChangeChannelReadonlyJobTest)
using namespace RocketChatRestApi;
ChangeChannelReadonlyJobTest::ChangeChannelReadonlyJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeChannelReadonlyJobTest::shouldHaveDefaultValue()
{
    ChangeChannelReadonlyJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.readOnly());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeChannelReadonlyJobTest::shouldGenerateRequest()
{
    ChangeChannelReadonlyJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.setReadOnly")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChangeChannelReadonlyJobTest::shouldGenerateJson()
{
    ChangeChannelReadonlyJob job;
    const QString roomId = QStringLiteral("foo1");
    bool readOnly = true;
    job.setRoomId(roomId);
    job.setReadOnly(readOnly);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"readOnly":true,"roomId":"%1"})").arg(roomId).toLatin1());

    readOnly = false;
    job.setReadOnly(readOnly);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"readOnly":false,"roomId":"%1"})").arg(roomId).toLatin1());
}

#include "moc_changechannelreadonlyjobtest.cpp"
