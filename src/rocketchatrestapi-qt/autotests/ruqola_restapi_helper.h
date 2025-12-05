/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "restapiabstractjob.h"

#include "restapimethod.h"

#include <QNetworkRequest>
#include <QTest>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
namespace RuqolaRestApiHelper
{
void verifyAuthentication(RestApiAbstractJob *job, QNetworkRequest &request)
{
    const QString authToken = u"foo"_s;
    const QString userId = u"user"_s;
    job->setUserId(userId);
    job->setAuthToken(authToken);
    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job->setRestApiMethod(&method);
    request = job->request();
    QCOMPARE(request.attribute(QNetworkRequest::HttpPipeliningAllowedAttribute).toBool(), true);
    QCOMPARE(request.attribute(QNetworkRequest::Http2AllowedAttribute).toBool(), true);
    QCOMPARE(request.rawHeader("X-Auth-Token"_ba), authToken.toLocal8Bit());
    QCOMPARE(request.rawHeader("X-User-Id"_ba), userId.toLocal8Bit());
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
}
