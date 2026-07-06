/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "deletemessagefilejobtest.h"

#include "chat/deletemessagefilejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QTest>

using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(DeleteMessageFileJobTest)
using namespace RocketChatRestApi;
DeleteMessageFileJobTest::DeleteMessageFileJobTest(QObject *parent)
    : QObject(parent)
{
}

void DeleteMessageFileJobTest::shouldHaveDefaultValue()
{
    DeleteMessageFileJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.fileId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void DeleteMessageFileJobTest::shouldGenerateRequest()
{
    DeleteMessageFileJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/chat.delete"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void DeleteMessageFileJobTest::shouldGenerateJson()
{
    DeleteMessageFileJob job;
    const QByteArray fileId("foo1");
    job.setFileId(fileId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"fileId":"%1"})").arg(QLatin1StringView(fileId)).toLatin1());
}

void DeleteMessageFileJobTest::shouldNotStarting()
{
    DeleteMessageFileJob job;

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
    const QByteArray fileId("foo1");
    job.setFileId(fileId);
    QVERIFY(job.canStart());
}

#include "moc_deletemessagefilejobtest.cpp"
