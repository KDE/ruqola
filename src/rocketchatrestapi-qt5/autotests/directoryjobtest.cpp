/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "directoryjobtest.h"
#include "misc/directoryjob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <restapimethod.h>
QTEST_GUILESS_MAIN(DirectoryJobTest)
using namespace RocketChatRestApi;
DirectoryJobTest::DirectoryJobTest(QObject *parent)
    : QObject(parent)
{
}

void DirectoryJobTest::shouldHaveDefaultValue()
{
    DirectoryJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.hasQueryParameterSupport());
}

void DirectoryJobTest::shouldGenerateRequest()
{
    DirectoryJob job;
    {
        QNetworkRequest request = QNetworkRequest(QUrl());
        verifyAuthentication(&job, request);
        QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/directory")));
    }
}

void DirectoryJobTest::shouldNotStarting()
{
    DirectoryJob job;
    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager networkAccessManager;
    job.setNetworkAccessManager(&networkAccessManager);
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    DirectoryJob::DirectoryInfo info;
    info.searchType = DirectoryJob::Rooms;
    job.setDirectoryInfo(info);
    QVERIFY(job.canStart());
}
