/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "updatemessagejobtest.h"
#include "chat/updatemessagejob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(UpdateMessageJobTest)
using namespace RocketChatRestApi;
UpdateMessageJobTest::UpdateMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void UpdateMessageJobTest::shouldHaveDefaultValue()
{
    UpdateMessageJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(job.updatedText().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void UpdateMessageJobTest::shouldGenerateRequest()
{
    UpdateMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.update")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void UpdateMessageJobTest::shouldGenerateJson()
{
    UpdateMessageJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString messageId = QStringLiteral("topic1");
    const QString updatedText = QStringLiteral("topic1");
    job.setRoomId(roomId);
    job.setMessageId(messageId);
    job.setUpdatedText(updatedText);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral("{\"msgId\":\"%2\",\"roomId\":\"%1\",\"text\":\"%3\"}")
             .arg(roomId, messageId, updatedText).toLatin1());
}

void UpdateMessageJobTest::shouldNotStarting()
{
    UpdateMessageJob job;

    RestApiMethod *method = new RestApiMethod;
    method->setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(method);

    QNetworkAccessManager *mNetworkAccessManager = new QNetworkAccessManager;
    job.setNetworkAccessManager(mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString roomId = QStringLiteral("foo1");
    job.setRoomId(roomId);
    QVERIFY(!job.canStart());
    const QString text = QStringLiteral("topic1");
    job.setMessageId(text);
    QVERIFY(job.canStart());
    QVERIFY(job.updatedText().isEmpty());
    QVERIFY(job.canStart());
    job.setUpdatedText(QStringLiteral("too"));
    QVERIFY(job.canStart());

    delete method;
    delete mNetworkAccessManager;
}
