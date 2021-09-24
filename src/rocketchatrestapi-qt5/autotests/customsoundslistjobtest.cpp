/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

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

#include "customsoundslistjobtest.h"
#include "custom/customsoundslistjob.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(CustomSoundsListJobTest)
using namespace RocketChatRestApi;
CustomSoundsListJobTest::CustomSoundsListJobTest(QObject *parent)
    : QObject(parent)
{
}

void CustomSoundsListJobTest::shouldHaveDefaultValue()
{
    CustomSoundsListJob job;
    QVERIFY(!job.restApiMethod());
    QVERIFY(!job.networkAccessManager());
    QVERIFY(!job.start());
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.authToken().isEmpty());
    QVERIFY(job.userId().isEmpty());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.hasQueryParameterSupport());
}

void CustomSoundsListJobTest::shouldGenerateRequest()
{
    {
        CustomSoundsListJob job;
        const QString authToken = QStringLiteral("foo");
        const QString userId = QStringLiteral("user");
        job.setUserId(userId);
        job.setAuthToken(authToken);
        RestApiMethod method;
        method.setServerUrl(QStringLiteral("http://www.kde.org"));
        job.setRestApiMethod(&method);
        const QNetworkRequest request = job.request();
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/custom-sounds.list")));
        QCOMPARE(request.rawHeader(QByteArrayLiteral("X-Auth-Token")), authToken.toLocal8Bit());
        QCOMPARE(request.rawHeader(QByteArrayLiteral("X-User-Id")), userId.toLocal8Bit());
    }
    {
        CustomSoundsListJob job;
        const QString authToken = QStringLiteral("foo");
        const QString userId = QStringLiteral("user");
        job.setUserId(userId);
        job.setAuthToken(authToken);
        RestApiMethod method;
        method.setServerUrl(QStringLiteral("http://www.kde.org"));
        job.setRestApiMethod(&method);

        QMap<QString, RocketChatRestApi::QueryParameters::SortOrder> map;
        map.insert(QStringLiteral("name"), RocketChatRestApi::QueryParameters::SortOrder::Ascendant);
        RocketChatRestApi::QueryParameters parameters;
        parameters.setSorting(map);
        parameters.setOffset(1);
        parameters.setCount(3);
        parameters.setSearchString(QStringLiteral("bla"));

        job.setQueryParameters(parameters);
        const QNetworkRequest request = job.request();
        QCOMPARE(request.url().query(),
                 QStringLiteral("count=3&offset=1&query=%7B%22name%22:%7B%22$regex%22:%22bla%22,%22$options%22:%22i%22%7D%7D&sort=%7B%22name%22:1%7D"));
        QCOMPARE(request.rawHeader(QByteArrayLiteral("X-Auth-Token")), authToken.toLocal8Bit());
        QCOMPARE(request.rawHeader(QByteArrayLiteral("X-User-Id")), userId.toLocal8Bit());
    }
}
