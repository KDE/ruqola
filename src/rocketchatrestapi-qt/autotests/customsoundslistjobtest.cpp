/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundslistjobtest.h"
using namespace Qt::Literals::StringLiterals;

#include "custom/customsoundslistjob.h"
#include "restapimethod.h"
#include <QTest>

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
    QVERIFY(job.authCode().isEmpty());
    QVERIFY(job.authMethod().isEmpty());
    QVERIFY(job.userId().isEmpty());
    QVERIFY(!job.restApiLogger());
    QVERIFY(job.hasQueryParameterSupport());
    QVERIFY(!job.requireTwoFactorAuthentication());
}

void CustomSoundsListJobTest::shouldGenerateRequest()
{
    {
        CustomSoundsListJob job;
        const QString authToken = u"foo"_s;
        const QString userId = u"user"_s;
        job.setUserId(userId);
        job.setAuthToken(authToken);
        RestApiMethod method;
        method.setServerUrl(u"http://www.kde.org"_s);
        job.setRestApiMethod(&method);
        const QNetworkRequest request = job.request();
        QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/custom-sounds.list"_s));
        QCOMPARE(request.rawHeader("X-Auth-Token"_ba), authToken.toLocal8Bit());
        QCOMPARE(request.rawHeader("X-User-Id"_ba), userId.toLocal8Bit());
    }
    {
        CustomSoundsListJob job;
        const QString authToken = u"foo"_s;
        const QString userId = u"user"_s;
        job.setUserId(userId);
        job.setAuthToken(authToken);
        RestApiMethod method;
        method.setServerUrl(u"http://www.kde.org"_s);
        job.setRestApiMethod(&method);

        QMap<QString, RocketChatRestApi::QueryParameters::SortOrder> map;
        map.insert(u"name"_s, RocketChatRestApi::QueryParameters::SortOrder::Ascendant);
        RocketChatRestApi::QueryParameters parameters;
        parameters.setSorting(map);
        parameters.setOffset(1);
        parameters.setCount(3);
        parameters.setSearchString(u"bla"_s);

        job.setQueryParameters(parameters);
        const QNetworkRequest request = job.request();
        QCOMPARE(request.url().query(),
                 u"count=3&offset=1&query=%7B%22name%22:%7B%22$regex%22:%22bla%22,%22$options%22:%22i%22%7D%7D&sort=%7B%22name%22:1%7D"_s);
        QCOMPARE(request.rawHeader("X-Auth-Token"_ba), authToken.toLocal8Bit());
        QCOMPARE(request.rawHeader("X-User-Id"_ba), userId.toLocal8Bit());
    }
}

#include "moc_customsoundslistjobtest.cpp"
