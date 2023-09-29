/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomscleanhistoryjobtest.h"
#include "rooms/roomscleanhistoryjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(RoomsCleanHistoryJobTest)
using namespace RocketChatRestApi;
RoomsCleanHistoryJobTest::RoomsCleanHistoryJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomsCleanHistoryJobTest::shouldHaveDefaultValue()
{
    RoomsCleanHistoryJob job;
    QVERIFY(!job.cleanHistoryInfo().isValid());
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());

    RoomsCleanHistoryJob::CleanHistoryInfo info;
    QVERIFY(!info.isValid());
    QVERIFY(!info.inclusive);
    QVERIFY(!info.excludePinned);
    QVERIFY(!info.filesOnly);
    QVERIFY(!info.ignoreThreads);
    QVERIFY(!info.ignoreDiscussion);
    QVERIFY(info.users.isEmpty());
    QVERIFY(info.roomId.isEmpty());
    QVERIFY(!info.oldest.isValid());
    QVERIFY(!info.latest.isValid());
}

void RoomsCleanHistoryJobTest::shouldGenerateRequest()
{
    RoomsCleanHistoryJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.cleanHistory")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void RoomsCleanHistoryJobTest::shouldGenerateJson()
{
    RoomsCleanHistoryJob job;
    RoomsCleanHistoryJob::CleanHistoryInfo info;
    const QString roomId = QStringLiteral("room1");
    info.roomId = roomId;
    info.latest = QDateTime(QDate(2020, 12, 3), QTime(5, 7, 50));
    info.oldest = QDateTime(QDate(2020, 3, 3), QTime(5, 7, 50));
    job.setCleanHistoryInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"latest":"2020-12-03T05:07:50.000","oldest":"2020-03-03T05:07:50.000","roomId":"%1"})").arg(roomId).toLatin1());
    info.inclusive = true;
    job.setCleanHistoryInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(R"({"inclusive":true,"latest":"2020-12-03T05:07:50.000","oldest":"2020-03-03T05:07:50.000","roomId":"%1"})").arg(roomId).toLatin1());
    info.ignoreThreads = true;
    job.setCleanHistoryInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"ignoreThreads":true,"inclusive":true,"latest":"2020-12-03T05:07:50.000","oldest":"2020-03-03T05:07:50.000","roomId":"%1"})")
                 .arg(roomId)
                 .toLatin1());
    const QStringList users = {QStringLiteral("bla"), QStringLiteral("bli")};
    info.users = users;
    job.setCleanHistoryInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(
            R"({"ignoreThreads":true,"inclusive":true,"latest":"2020-12-03T05:07:50.000","oldest":"2020-03-03T05:07:50.000","roomId":"%1","users":["bla","bli"]})")
            .arg(roomId)
            .toLatin1());
    info.ignoreDiscussion = true;
    job.setCleanHistoryInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(
            R"({"ignoreDiscussion":true,"ignoreThreads":true,"inclusive":true,"latest":"2020-12-03T05:07:50.000","oldest":"2020-03-03T05:07:50.000","roomId":"%1","users":["bla","bli"]})")
            .arg(roomId)
            .toLatin1());
}

void RoomsCleanHistoryJobTest::shouldNotStarting()
{
    RoomsCleanHistoryJob job;

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
    RoomsCleanHistoryJob::CleanHistoryInfo info;
    const QString roomId = QStringLiteral("room1");
    info.roomId = roomId;
    job.setCleanHistoryInfo(info);
    QVERIFY(!job.canStart());

    info.latest = QDateTime(QDate(2020, 12, 3), QTime(5, 7, 50));
    info.oldest = QDateTime(QDate(2020, 3, 3), QTime(5, 7, 50));
    job.setCleanHistoryInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_roomscleanhistoryjobtest.cpp"
