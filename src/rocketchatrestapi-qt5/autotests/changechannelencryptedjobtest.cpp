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

#include "changechannelencryptedjobtest.h"
#include "channels/changechannelencryptedjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChangeChannelEncryptedJobTest)
using namespace RocketChatRestApi;
ChangeChannelEncryptedJobTest::ChangeChannelEncryptedJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeChannelEncryptedJobTest::shouldHaveDefaultValue()
{
    ChangeChannelEncryptedJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.roomId().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ChangeChannelEncryptedJobTest::shouldGenerateRequest()
{
    ChangeChannelEncryptedJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/channels.setEncrypted")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChangeChannelEncryptedJobTest::shouldGenerateJson()
{
    ChangeChannelEncryptedJob job;
    const QString roomId = QStringLiteral("foo1");
    job.setRoomId(roomId);
    job.setEncrypted(true);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"encrypted\":true,\"roomId\":\"%1\"}").arg(roomId).toLatin1());
    job.setEncrypted(false);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"encrypted\":false,\"roomId\":\"%1\"}").arg(roomId).toLatin1());
}
