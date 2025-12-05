/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomscleanhistoryjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "rooms/roomscleanhistoryjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
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
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.cleanHistory"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RoomsCleanHistoryJobTest::shouldGenerateJson()
{
    RoomsCleanHistoryJob job;
    RoomsCleanHistoryJob::CleanHistoryInfo info;
    const QByteArray roomId("room1");
    info.roomId = roomId;
    info.latest = QDateTime(QDate(2020, 12, 3), QTime(5, 7, 50));
    info.oldest = QDateTime(QDate(2020, 3, 3), QTime(5, 7, 50));
    job.setCleanHistoryInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(R"({"latest":"2020-12-03T05:07:50.000","oldest":"2020-03-03T05:07:50.000","roomId":"%1"})").arg(QLatin1StringView(roomId)).toLatin1());
    info.inclusive = true;
    job.setCleanHistoryInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"inclusive":true,"latest":"2020-12-03T05:07:50.000","oldest":"2020-03-03T05:07:50.000","roomId":"%1"})")
                 .arg(QLatin1StringView(roomId))
                 .toLatin1());
    info.ignoreThreads = true;
    job.setCleanHistoryInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"ignoreThreads":true,"inclusive":true,"latest":"2020-12-03T05:07:50.000","oldest":"2020-03-03T05:07:50.000","roomId":"%1"})")
                 .arg(QLatin1StringView(roomId))
                 .toLatin1());
    const QStringList users = {u"bla"_s, u"bli"_s};
    info.users = users;
    job.setCleanHistoryInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(
            R"({"ignoreThreads":true,"inclusive":true,"latest":"2020-12-03T05:07:50.000","oldest":"2020-03-03T05:07:50.000","roomId":"%1","users":["bla","bli"]})")
            .arg(QLatin1StringView(roomId))
            .toLatin1());
    info.ignoreDiscussion = true;
    job.setCleanHistoryInfo(info);
    QCOMPARE(
        job.json().toJson(QJsonDocument::Compact),
        QStringLiteral(
            R"({"ignoreDiscussion":true,"ignoreThreads":true,"inclusive":true,"latest":"2020-12-03T05:07:50.000","oldest":"2020-03-03T05:07:50.000","roomId":"%1","users":["bla","bli"]})")
            .arg(QLatin1StringView(roomId))
            .toLatin1());
}

void RoomsCleanHistoryJobTest::shouldNotStarting()
{
    RoomsCleanHistoryJob job;

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
    RoomsCleanHistoryJob::CleanHistoryInfo info;
    const QByteArray roomId("room1");
    info.roomId = roomId;
    job.setCleanHistoryInfo(info);
    QVERIFY(!job.canStart());

    info.latest = QDateTime(QDate(2020, 12, 3), QTime(5, 7, 50));
    info.oldest = QDateTime(QDate(2020, 3, 3), QTime(5, 7, 50));
    job.setCleanHistoryInfo(info);
    QVERIFY(job.canStart());
}

#include "moc_roomscleanhistoryjobtest.cpp"
