/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "restapiabstractjob.h"
#include "restapimethod.h"

#include <QNetworkRequest>
#include <QTest>
using namespace RocketChatRestApi;
void verifyAuthentication(RestApiAbstractJob *job, QNetworkRequest &request)
{
    const QString authToken = QStringLiteral("foo");
    const QString userId = QStringLiteral("user");
    job->setUserId(userId);
    job->setAuthToken(authToken);
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job->setRestApiMethod(&method);
    request = job->request();
    QCOMPARE(request.attribute(QNetworkRequest::HttpPipeliningAllowedAttribute).toBool(), true);
    QCOMPARE(request.attribute(QNetworkRequest::Http2AllowedAttribute).toBool(), true);
    QCOMPARE(request.rawHeader(QByteArrayLiteral("X-Auth-Token")), authToken.toLocal8Bit());
    QCOMPARE(request.rawHeader(QByteArrayLiteral("X-User-Id")), userId.toLocal8Bit());
}

void verifyDefaultValue(RestApiAbstractJob *job)
{
    QVERIFY(!job->restApiMethod());
    QVERIFY(!job->networkAccessManager());
    QVERIFY(!job->start());
    QVERIFY(job->authToken().isEmpty());
    QVERIFY(job->userId().isEmpty());
    QVERIFY(!job->restApiLogger());
}
