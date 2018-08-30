/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "changegroupsnamejobtest.h"
#include "groups/changegroupsnamejob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChangeGroupsNameJobTest)

ChangeGroupsNameJobTest::ChangeGroupsNameJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeGroupsNameJobTest::shouldHaveDefaultValue()
{
    ChangeGroupsNameJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.name().isEmpty());
    QVERIFY(job.roomId().isEmpty());
}

void ChangeGroupsNameJobTest::shouldGenerateRequest()
{
    ChangeGroupsNameJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.rename")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChangeGroupsNameJobTest::shouldGenerateJson()
{
    ChangeGroupsNameJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString name = QStringLiteral("topic1");
    job.setRoomId(roomId);
    job.setName(name);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"name\":\"%1\",\"roomId\":\"%2\"}").arg(name).arg(roomId).toLatin1());
}
