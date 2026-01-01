/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changechannelnamejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "channels/changechannelnamejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChangeChannelNameJobTest)
using namespace RocketChatRestApi;
ChangeChannelNameJobTest::ChangeChannelNameJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeChannelNameJobTest::shouldHaveDefaultValue()
{
    ChangeChannelNameJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.name().isEmpty());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeChannelNameJobTest::shouldGenerateRequest()
{
    ChangeChannelNameJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.rename"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChangeChannelNameJobTest::shouldGenerateJson()
{
    ChangeChannelNameJob job;
    const QString roomId = u"foo1"_s;
    const QString name = u"topic1"_s;
    job.setRoomId(roomId);
    job.setName(name);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"name":"%1","roomId":"%2"})").arg(name, roomId).toLatin1());
}

#include "moc_changechannelnamejobtest.cpp"
