/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "roomsexportjobtest.h"
#include "rooms/roomsexportjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
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
    verifyDefaultValue(&job);
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
}

void RoomsExportJobTest::shouldGenerateRequest()
{
    RoomsExportJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.export")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void RoomsExportJobTest::shouldGenerateJson()
{
    RoomsExportJob job;
    RoomsExportJob::RoomsExportInfo info;
    //TODO
    const QString roomId = QStringLiteral("room1");
    info.roomId = roomId;
    info.exportAs = RoomsExportJob::RoomsExportInfo::ExportAs::File;

    info.dateFrom = QDateTime(QDate(2020, 12, 3), QTime(5, 7, 50)).toUTC();
    info.dateTo = QDateTime(QDate(2020, 3, 3), QTime(5, 7, 50)).toUTC();
    job.setRoomExportInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"dateFrom":"2020-12-03T04:07:50.000Z","dateTo":"2020-03-03T04:07:50.000Z","rid":"%1","type":"file"})").arg(roomId).toLatin1());
//    info.inclusive = true;
//    job.setCleanHistoryInfo(info);
//    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"inclusive":true,"latest":"2020-12-03T04:07:50.000Z","oldest":"2020-03-03T04:07:50.000Z","roomId":"%1"})").arg(roomId).toLatin1());
//    info.ignoreThreads = true;
//    job.setCleanHistoryInfo(info);
//    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"ignoreThreads":true,"inclusive":true,"latest":"2020-12-03T04:07:50.000Z","oldest":"2020-03-03T04:07:50.000Z","roomId":"%1"})").arg(roomId).toLatin1());
//    const QStringList users = {QStringLiteral("bla"), QStringLiteral("bli")};
//    info.users = users;
//    job.setCleanHistoryInfo(info);
//    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"ignoreThreads":true,"inclusive":true,"latest":"2020-12-03T04:07:50.000Z","oldest":"2020-03-03T04:07:50.000Z","roomId":"room1","users":["bla","bli"]})").arg(roomId).toLatin1());
}

void RoomsExportJobTest::shouldNotStarting()
{
    RoomsExportJob job;

    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    RoomsExportJob::RoomsExportInfo info;
    const QString roomId = QStringLiteral("room1");
    info.roomId = roomId;
    job.setRoomExportInfo(info);
    QVERIFY(!job.canStart());

//    info.latest = QDateTime(QDate(2020, 12, 3), QTime(5, 7, 50));
//    info.oldest = QDateTime(QDate(2020, 3, 3), QTime(5, 7, 50));
//    job.setCleanHistoryInfo(info);
//    QVERIFY(job.canStart());
    //TODO
}
