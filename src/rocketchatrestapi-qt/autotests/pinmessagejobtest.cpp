/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "pinmessagejobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "chat/pinmessagejob.h"

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
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(job.pinMessage());
    QVERIFY(!job.hasQueryParameterSupport());
}

void PinMessageJobTest::shouldHaveMessageId()
{
    PinMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    QVERIFY(!job.canStart());
    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    job.setMessageId("bla"_ba);
    QVERIFY(job.canStart());
}

void PinMessageJobTest::shouldGeneratePinMessageRequest()
{
    PinMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.pinMessage"_s));
}

void PinMessageJobTest::shouldGenerateUnPinMessageRequest()
{
    PinMessageJob job;
    job.setPinMessage(false);
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.unPinMessage"_s));
}

void PinMessageJobTest::shouldGenerateJson()
{
    PinMessageJob job;
    const QByteArray messageId("foo1");
    job.setMessageId(messageId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"messageId":"%1"})").arg(QLatin1StringView(messageId)).toLatin1());
    job.setPinMessage(false);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"messageId":"%1"})").arg(QLatin1StringView(messageId)).toLatin1());
}

#include "moc_pinmessagejobtest.cpp"
