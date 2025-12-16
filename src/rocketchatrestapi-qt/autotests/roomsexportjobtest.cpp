/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsexportjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rooms/roomsexportjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(RoomsExportJobTest)
using namespace RocketChatRestApi;
RoomsExportJobTest::RoomsExportJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomsExportJobTest::shouldHaveDefaultValue()
{
    RoomsExportJob job;
    QVERIFY(!job.roomExportInfo().isValid());
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());

    RoomsExportJob::RoomsExportInfo info;
    QVERIFY(!info.isValid());
    QCOMPARE(info.fileFormat, RoomsExportJob::RoomsExportInfo::FileFormat::Unknown);
    QVERIFY(info.roomId.isEmpty());
    QVERIFY(info.toUsers.isEmpty());
    QVERIFY(info.toEmails.isEmpty());
    QVERIFY(info.subject.isEmpty());
    QVERIFY(info.messages.isEmpty());
    QVERIFY(!info.dateFrom.isValid());
    QVERIFY(!info.dateTo.isValid());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void RoomsExportJobTest::shouldGenerateRequest()
{
    RoomsExportJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.export"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RoomsExportJobTest::shouldGenerateJson()
{
    RoomsExportJob job;
    RoomsExportJob::RoomsExportInfo info;
    // TODO
    const QByteArray roomId("room1");
    info.roomId = roomId;
    info.exportAs = RoomsExportJob::RoomsExportInfo::ExportAs::File;

    info.dateFrom = QDateTime(QDate(2020, 12, 3), QTime(5, 7, 50));
    info.dateTo = QDateTime(QDate(2020, 3, 3), QTime(5, 7, 50));
    job.setRoomExportInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"dateFrom":"2020-12-03","dateTo":"2020-03-03","rid":"%1","type":"file"})").arg(QLatin1StringView(roomId)).toLatin1());
    //    info.inclusive = true;
    //    job.setCleanHistoryInfo(info);
    //    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
    //    QStringLiteral(R"({"inclusive":true,"latest":"2020-12-03T04:07:50.000Z","oldest":"2020-03-03T04:07:50.000Z","roomId":"%1"})").arg(roomId).toLatin1());
    //    info.ignoreThreads = true;
    //    job.setCleanHistoryInfo(info);
    //    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
    //    QStringLiteral(R"({"ignoreThreads":true,"inclusive":true,"latest":"2020-12-03T04:07:50.000Z","oldest":"2020-03-03T04:07:50.000Z","roomId":"%1"})").arg(roomId).toLatin1());
    //    const QStringList users = {u"bla"_s, u"bli"_s};
    //    info.users = users;
    //    job.setCleanHistoryInfo(info);
    //    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
    //    QStringLiteral(R"({"ignoreThreads":true,"inclusive":true,"latest":"2020-12-03T04:07:50.000Z","oldest":"2020-03-03T04:07:50.000Z","roomId":"room1","users":["bla","bli"]})").arg(roomId).toLatin1());
}

void RoomsExportJobTest::shouldNotStarting()
{
    RoomsExportJob job;

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
    RoomsExportJob::RoomsExportInfo info;
    const QByteArray roomId("room1");
    info.roomId = roomId;
    job.setRoomExportInfo(info);
    QVERIFY(!job.canStart());

    //    info.latest = QDateTime(QDate(2020, 12, 3), QTime(5, 7, 50));
    //    info.oldest = QDateTime(QDate(2020, 3, 3), QTime(5, 7, 50));
    //    job.setCleanHistoryInfo(info);
    //    QVERIFY(job.canStart());
    // TODO
}

#include "moc_roomsexportjobtest.cpp"
