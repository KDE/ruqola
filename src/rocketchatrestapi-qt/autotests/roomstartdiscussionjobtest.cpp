/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomstartdiscussionjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rooms/roomstartdiscussionjob.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(RoomStartDiscussionJobTest)
using namespace RocketChatRestApi;
RoomStartDiscussionJobTest::RoomStartDiscussionJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomStartDiscussionJobTest::shouldHaveDefaultValue()
{
    RoomStartDiscussionJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.parentRoomId().isEmpty());
    QVERIFY(job.parentMessageId().isEmpty());
    QVERIFY(job.discussionName().isEmpty());
    QVERIFY(job.replyMessage().isEmpty());
    QVERIFY(job.users().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
    QVERIFY(!job.encrypted());
}

void RoomStartDiscussionJobTest::shouldGenerateRequest()
{
    RoomStartDiscussionJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/rooms.createDiscussion"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void RoomStartDiscussionJobTest::shouldGenerateJson()
{
    RoomStartDiscussionJob job;
    const QByteArray pRid("foo1");
    job.setParentRoomId(pRid);
    const QString discussionName = u"bla"_s;
    job.setDiscussionName(discussionName);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"prid":"%1","t_name":"%2"})").arg(QLatin1StringView(pRid), discussionName).toLatin1());
    const QString replyMessage = u"Bli"_s;
    job.setReplyMessage(replyMessage);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"prid":"%1","reply":"%2","t_name":"%3"})").arg(QLatin1StringView(pRid), replyMessage, discussionName).toLatin1());

    const QList<QByteArray> users{QByteArrayLiteral("aaa"), QByteArrayLiteral("bbb"), "ddd"_ba};
    job.setUsers(users);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"prid":"%1","reply":"%2","t_name":"%3","users":["aaa","bbb","ddd"]})")
                 .arg(QLatin1StringView(pRid), replyMessage, discussionName)
                 .toLatin1());

    job.setEncrypted(true);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"encrypted":true,"prid":"%1","reply":"%2","t_name":"%3","users":["aaa","bbb","ddd"]})")
                 .arg(QLatin1StringView(pRid), replyMessage, discussionName)
                 .toLatin1());
}

void RoomStartDiscussionJobTest::shouldNotStarting()
{
    RoomStartDiscussionJob job;

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
    const QByteArray pRid("foo1");
    job.setParentRoomId(pRid);
    QVERIFY(!job.canStart());
    const QString discussionName = u"bla"_s;
    job.setDiscussionName(discussionName);
    QVERIFY(job.canStart());
}

#include "moc_roomstartdiscussionjobtest.cpp"
