/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "pinmessagejobtest.h"
#include "chat/pinmessagejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(PinMessageJobTest)
using namespace RocketChatRestApi;

PinMessageJobTest::PinMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void PinMessageJobTest::shouldHaveDefaultValue()
{
    PinMessageJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(job.pinMessage());
    QVERIFY(!job.hasQueryParameterSupport());
}

void PinMessageJobTest::shouldHaveMessageId()
{
    PinMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);
    QVERIFY(!job.canStart());
    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    job.setMessageId(QStringLiteral("bla"));
    QVERIFY(job.canStart());
}

void PinMessageJobTest::shouldGeneratePinMessageRequest()
{
    PinMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.pinMessage")));
}

void PinMessageJobTest::shouldGenerateUnPinMessageRequest()
{
    PinMessageJob job;
    job.setPinMessage(false);
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.unPinMessage")));
}

void PinMessageJobTest::shouldGenerateJson()
{
    PinMessageJob job;
    const QString messageId = QStringLiteral("foo1");
    job.setMessageId(messageId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"messageId":"%1"})").arg(messageId).toLatin1());
    job.setPinMessage(false);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"messageId":"%1"})").arg(messageId).toLatin1());
}

#include "moc_pinmessagejobtest.cpp"
