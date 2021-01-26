/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#include "serverinfojobtest.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"
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
    verifyDefaultValue(&job);
    QVERIFY(!job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
    QVERIFY(job.useDeprecatedVersion());
}

void ServerInfoJobTest::shouldGenerateRequest()
{
    {
        ServerInfoJob job;
        RestApiMethod method;
        method.setServerUrl(QStringLiteral("http://www.kde.org"));
        job.setRestApiMethod(&method);
        const QNetworkRequest request = job.request();
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/info")));
        QCOMPARE(request.attribute(QNetworkRequest::HttpPipeliningAllowedAttribute).toBool(), true);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
        QCOMPARE(request.attribute(QNetworkRequest::HTTP2AllowedAttribute).toBool(), true);
#else
        QCOMPARE(request.attribute(QNetworkRequest::Http2AllowedAttribute).toBool(), true);
#endif
    }
    {
        ServerInfoJob job;
        job.setUseDeprecatedVersion(false);
        RestApiMethod method;
        method.setServerUrl(QStringLiteral("http://www.kde.org"));
        job.setRestApiMethod(&method);
        const QNetworkRequest request = job.request();
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/info")));
        QCOMPARE(request.attribute(QNetworkRequest::HttpPipeliningAllowedAttribute).toBool(), true);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
        QCOMPARE(request.attribute(QNetworkRequest::HTTP2AllowedAttribute).toBool(), true);
#else
        QCOMPARE(request.attribute(QNetworkRequest::Http2AllowedAttribute).toBool(), true);
#endif
    }
}
