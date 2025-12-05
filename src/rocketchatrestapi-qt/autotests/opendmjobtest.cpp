/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "opendmjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "directmessage/opendmjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
QTEST_GUILESS_MAIN(OpenDmJobTest)
using namespace RocketChatRestApi;
OpenDmJobTest::OpenDmJobTest(QObject *parent)
    : QObject(parent)
{
}

void OpenDmJobTest::shouldHaveDefaultValue()
{
    OpenDmJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.directUserId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void OpenDmJobTest::shouldGenerateRequest()
{
    OpenDmJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/im.open"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void OpenDmJobTest::shouldGenerateJson()
{
    OpenDmJob job;
    const QString channelname = u"foo1"_s;
    job.setDirectUserId(channelname);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"roomId":"%1"})").arg(channelname).toLatin1());
}

void OpenDmJobTest::shouldNotStarting()
{
    OpenDmJob job;

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
    const QString username = u"foo1"_s;
    job.setDirectUserId(username);
    QVERIFY(job.canStart());
}

#include "moc_opendmjobtest.cpp"
