/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "pinmessagejobtest.h"
#include "ruqola_restapi_helper.h"
#include "chat/pinmessagejob.h"
#include <QTest>
#include <QJsonDocument>
#include <restapimethod.h>
#include <QTest>
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
}

void PinMessageJobTest::shouldHaveMessageId()
{
    PinMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    RestApiMethod *method = new RestApiMethod;
    method->setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(method);
    QVERIFY(!job.canStart());
    QNetworkAccessManager *mNetworkAccessManager = new QNetworkAccessManager;
    job.setNetworkAccessManager(mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    job.setMessageId(QStringLiteral("bla"));
    QVERIFY(job.canStart());
    delete method;
    delete mNetworkAccessManager;
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
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"messageId\":\"%1\"}").arg(messageId).toLatin1());
    job.setPinMessage(false);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"messageId\":\"%1\"}").arg(messageId).toLatin1());
}
