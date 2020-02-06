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

#include "translatemessagejobtest.h"
#include "autotranslate/translatemessagejob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(TranslateMessageJobTest)
using namespace RocketChatRestApi;
TranslateMessageJobTest::TranslateMessageJobTest(QObject *parent)
    : QObject(parent)
{
}

void TranslateMessageJobTest::shouldHaveDefaultValue()
{
    TranslateMessageJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.messageId().isEmpty());
    QVERIFY(job.targetLanguage().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void TranslateMessageJobTest::shouldGenerateRequest()
{
    TranslateMessageJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/autotranslate.translateMessage")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void TranslateMessageJobTest::shouldGenerateJson()
{
    TranslateMessageJob job;
    const QString messageId = QStringLiteral("foo1");
    job.setMessageId(messageId);
    const QString targetLanguage = QStringLiteral("bla");
    job.setTargetLanguage(targetLanguage);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"messageId\":\"%1\",\"targetLanguage\":\"%2\"}").arg(messageId).arg(targetLanguage).toLatin1());
}

void TranslateMessageJobTest::shouldNotStarting()
{
    TranslateMessageJob job;

    auto *method = new RestApiMethod;
    method->setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(method);

    auto *mNetworkAccessManager = new QNetworkAccessManager;
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
    const QString targetLanguage = QStringLiteral("bla");
    job.setTargetLanguage(targetLanguage);
    QVERIFY(job.canStart());

    delete method;
    delete mNetworkAccessManager;
}
