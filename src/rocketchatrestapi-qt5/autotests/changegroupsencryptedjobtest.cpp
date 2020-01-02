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

#include "changegroupsencryptedjobtest.h"
#include "groups/changegroupsencryptedjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChangeGroupsEncryptedJobTest)
using namespace RocketChatRestApi;
ChangeGroupsEncryptedJobTest::ChangeGroupsEncryptedJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeGroupsEncryptedJobTest::shouldHaveDefaultValue()
{
    ChangeGroupsEncryptedJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.encrypted());
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeGroupsEncryptedJobTest::shouldGenerateRequest()
{
    ChangeGroupsEncryptedJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.setEncrypted")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChangeGroupsEncryptedJobTest::shouldGenerateJson()
{
    ChangeGroupsEncryptedJob job;
    const QString roomId = QStringLiteral("foo1");
    bool encrypted = true;
    job.setRoomId(roomId);
    job.setEncrypted(encrypted);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"encrypted\":true,\"roomId\":\"%1\"}").arg(roomId).toLatin1());

    encrypted = false;
    job.setEncrypted(encrypted);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"encrypted\":false,\"roomId\":\"%1\"}").arg(roomId).toLatin1());
}
