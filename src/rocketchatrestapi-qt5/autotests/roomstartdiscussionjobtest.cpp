/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

#include "roomstartdiscussionjobtest.h"
#include "rooms/roomstartdiscussionjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(RoomStartDiscussionJobTest)
using namespace RocketChatRestApi;
RoomStartDiscussionJobTest::RoomStartDiscussionJobTest(QObject *parent)
    : QObject(parent)
{
}

void RoomStartDiscussionJobTest::shouldHaveDefaultValue()
{
    RoomStartDiscussionJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.parentRoomId().isEmpty());
    QVERIFY(job.parentMessageId().isEmpty());
    QVERIFY(job.discussionName().isEmpty());
    QVERIFY(job.replyMessage().isEmpty());
    QVERIFY(job.users().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void RoomStartDiscussionJobTest::shouldGenerateRequest()
{
    RoomStartDiscussionJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.createDiscussion")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void RoomStartDiscussionJobTest::shouldGenerateJson()
{
    RoomStartDiscussionJob job;
    const QString pRid = QStringLiteral("foo1");
    job.setParentRoomId(pRid);
    const QString discussionName = QStringLiteral("bla");
    job.setDiscussionName(discussionName);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"prid\":\"%1\",\"t_name\":\"%2\"}").arg(pRid).arg(discussionName).toLatin1());
    const QString replyMessage = QStringLiteral("Bli");
    job.setReplyMessage(replyMessage);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"prid\":\"%1\",\"reply\":\"%2\",\"t_name\":\"%3\"}").arg(pRid).arg(replyMessage).arg(discussionName).toLatin1());

    const QStringList users{QStringLiteral("aaa"), QStringLiteral("bbb"), QStringLiteral("ddd")};
    job.setUsers(users);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"prid\":\"%1\",\"reply\":\"%2\",\"t_name\":\"%3\",\"users\":[\"aaa\",\"bbb\",\"ddd\"]}")
             .arg(pRid).arg(replyMessage).arg(discussionName).arg(QStringLiteral("bla")).toLatin1());
}

void RoomStartDiscussionJobTest::shouldNotStarting()
{
    RoomStartDiscussionJob job;

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
    const QString pRid = QStringLiteral("foo1");
    job.setParentRoomId(pRid);
    QVERIFY(!job.canStart());
    const QString discussionName = QStringLiteral("bla");
    job.setDiscussionName(discussionName);
    QVERIFY(job.canStart());
}
