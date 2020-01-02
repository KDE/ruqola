/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

#include "changegroupsreadonlyjobtest.h"
#include "groups/changegroupsreadonlyjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChangeGroupsReadonlyJobTest)
using namespace RocketChatRestApi;
ChangeGroupsReadonlyJobTest::ChangeGroupsReadonlyJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeGroupsReadonlyJobTest::shouldHaveDefaultValue()
{
    ChangeGroupsReadonlyJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.readOnly());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeGroupsReadonlyJobTest::shouldGenerateRequest()
{
    ChangeGroupsReadonlyJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.setReadOnly")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChangeGroupsReadonlyJobTest::shouldGenerateJson()
{
    ChangeGroupsReadonlyJob job;
    const QString roomId = QStringLiteral("foo1");
    bool readOnly = true;
    job.setRoomId(roomId);
    job.setReadOnly(readOnly);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"readOnly\":true,\"roomId\":\"%1\"}").arg(roomId).toLatin1());

    readOnly = false;
    job.setReadOnly(readOnly);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"readOnly\":false,\"roomId\":\"%1\"}").arg(roomId).toLatin1());
}
