/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changechanneldescriptionjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "channels/changechanneldescriptionjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ChangeChannelDescriptionJobTest)
using namespace RocketChatRestApi;
ChangeChannelDescriptionJobTest::ChangeChannelDescriptionJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeChannelDescriptionJobTest::shouldNotStarting()
{
    ChangeChannelDescriptionJob job;

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
    QVERIFY(!job.canStart());
    const QString roomId = u"foo1"_s;
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

void ChangeChannelDescriptionJobTest::shouldHaveDefaultValue()
{
    ChangeChannelDescriptionJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.description().isEmpty());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeChannelDescriptionJobTest::shouldGenerateRequest()
{
    ChangeChannelDescriptionJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/channels.setDescription"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void ChangeChannelDescriptionJobTest::shouldGenerateJson()
{
    ChangeChannelDescriptionJob job;
    const QString roomId = u"foo1"_s;
    const QString description = u"topic1"_s;
    job.setRoomId(roomId);
    job.setDescription(description);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"description":"%1","roomId":"%2"})").arg(description, roomId).toLatin1());
}

#include "moc_changechanneldescriptionjobtest.cpp"
