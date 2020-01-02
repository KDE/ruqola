/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "reactonmessagejobtest.h"
#include "chat/reactonmessagejob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(ReactOnMessageJobTest)
using namespace RocketChatRestApi;
ReactOnMessageJobTest::ReactOnMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void ReactOnMessageJobTest::shouldHaveDefaultValue()
{
    ReactOnMessageJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(job.emoji().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ReactOnMessageJobTest::shouldGenerateRequest()
{
    ReactOnMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.react")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ReactOnMessageJobTest::shouldGenerateJson()
{
    ReactOnMessageJob job;
    const QString messageid = QStringLiteral("foo1");
    const QString emoji = QStringLiteral("topic1");
    job.setMessageId(messageid);
    job.setEmoji(emoji);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"emoji\":\"%1\",\"messageId\":\"%2\",\"shouldReact\":true}").arg(emoji, messageid).toLatin1());

    job.setShouldReact(false);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"emoji\":\"%1\",\"messageId\":\"%2\",\"shouldReact\":false}").arg(emoji, messageid).toLatin1());
}

void ReactOnMessageJobTest::shouldNotStarting()
{
    ReactOnMessageJob job;

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
    const QString messageId = QStringLiteral("foo1");
    job.setMessageId(messageId);
    QVERIFY(!job.canStart());
    const QString emoji = QStringLiteral("topic1");
    job.setEmoji(emoji);
    QVERIFY(job.canStart());

    delete method;
    delete mNetworkAccessManager;
}
