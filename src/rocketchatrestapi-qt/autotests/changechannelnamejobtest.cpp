/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changechannelnamejobtest.h"
#include "channels/changechannelnamejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ChangeChannelNameJobTest)
using namespace RocketChatRestApi;
ChangeChannelNameJobTest::ChangeChannelNameJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeChannelNameJobTest::shouldHaveDefaultValue()
{
    ChangeChannelNameJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.name().isEmpty());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeChannelNameJobTest::shouldGenerateRequest()
{
    ChangeChannelNameJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.rename")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChangeChannelNameJobTest::shouldGenerateJson()
{
    ChangeChannelNameJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString name = QStringLiteral("topic1");
    job.setRoomId(roomId);
    job.setName(name);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"name":"%1","roomId":"%2"})").arg(name, roomId).toLatin1());
}

#include "moc_changechannelnamejobtest.cpp"
