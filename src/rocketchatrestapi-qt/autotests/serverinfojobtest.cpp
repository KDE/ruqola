/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverinfojobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
#include <QTest>

#include "serverinfojob.h"
#include <QTest>

QTEST_GUILESS_MAIN(ServerInfoJobTest)
using namespace RocketChatRestApi;
ServerInfoJobTest::ServerInfoJobTest(QObject *parent)
    : QObject(parent)
{
}

void ServerInfoJobTest::shouldHaveDefaultValue()
{
    ServerInfoJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ServerInfoJobTest::shouldGenerateRequest()
{
    ServerInfoJob job;
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/info"_s));
    QCOMPARE(request.attribute(QNetworkRequest::HttpPipeliningAllowedAttribute).toBool(), true);
    QCOMPARE(request.attribute(QNetworkRequest::Http2AllowedAttribute).toBool(), true);
}

#include "moc_serverinfojobtest.cpp"
