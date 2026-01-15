/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsmediaconfirmjobtest.h"

#include "restapimethod.h"
#include "rooms/roomsmediaconfirmjob.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(RoomsMediaConfirmJobTest)
using namespace RocketChatRestApi;
RoomsMediaConfirmJobTest::RoomsMediaConfirmJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomsMediaConfirmJobTest::shouldHaveDefaultValue()
{
    RoomsMediaConfirmJob job;
    QVERIFY(job.fileId().isEmpty());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.description().isEmpty());
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void RoomsMediaConfirmJobTest::shouldGenerateRequest()
{
    {
        RoomsMediaConfirmJob job;
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.mediaConfirm//"_s));
        QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
    }
    {
        RoomsMediaConfirmJob job;
        job.setRoomId("roomid1"_ba);
        job.setFileId("fileId1"_ba);
        QNetworkRequest request = QNetworkRequest(QUrl());
        RuqolaRestApiHelper::verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.mediaConfirm/roomid1/fileId1"_s));
        QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
    }
}

void RoomsMediaConfirmJobTest::shouldGenerateJson()
{
    RoomsMediaConfirmJob job;

    const QString description(u"descr"_s);
    job.setDescription(description);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"description":"%1"})").arg(description).toLatin1());
}

void RoomsMediaConfirmJobTest::shouldNotStarting()
{
    RoomsMediaConfirmJob job;

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

    const QByteArray roomId("room1");
    job.setRoomId(roomId);
    QVERIFY(!job.canStart());
    const QByteArray fileId("file25"_ba);
    job.setFileId(fileId);
    QVERIFY(job.canStart());
}

#include "moc_roomsmediaconfirmjobtest.cpp"
