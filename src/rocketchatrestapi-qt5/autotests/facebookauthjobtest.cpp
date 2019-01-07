/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "facebookauthjobtest.h"
#include "authentication/facebookauthjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(FacebookAuthJobTest)
using namespace RocketChatRestApi;
FacebookAuthJobTest::FacebookAuthJobTest(QObject *parent)
    : QObject(parent)
{
}

void FacebookAuthJobTest::shouldHaveDefaultValue()
{
    FacebookAuthJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(job.secret().isEmpty());
    QVERIFY(job.accessToken().isEmpty());
    QCOMPARE(job.expireTokenInSeconds(), -1);
}

void FacebookAuthJobTest::shouldGenerateRequest()
{
    FacebookAuthJob job;

    RestApiMethod *method = new RestApiMethod;
    method->setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(method);
    const QNetworkRequest request = job.request();
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/login")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
    delete method;
}

void FacebookAuthJobTest::shouldGenerateJson()
{
    FacebookAuthJob job;
    const QString secret = QStringLiteral("secret:");
    job.setSecret(secret);

    const QString accessToken = QStringLiteral("accessToken");
    job.setAccessToken(accessToken);

    const int expireToken = 300;
    job.setExpireTokenInSeconds(expireToken);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"accessToken\":\"%1\",\"expiresIn\":300,\"secret\":\"%2\",\"serviceName\":\"facebook\"}")
             .arg(accessToken, secret).toLatin1());
}

void FacebookAuthJobTest::shouldNotStarting()
{
    FacebookAuthJob job;

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
    const QString secret = QStringLiteral("secret:");
    job.setSecret(secret);
    QVERIFY(!job.canStart());

    const QString accessToken = QStringLiteral("accessToken");
    job.setAccessToken(accessToken);
    QVERIFY(!job.canStart());

    const int expireToken = 300;
    job.setExpireTokenInSeconds(expireToken);
    QVERIFY(job.canStart());

    delete method;
    delete mNetworkAccessManager;
}
