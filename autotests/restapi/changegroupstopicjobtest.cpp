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

#include "changegroupstopicjobtest.h"
#include "groups/changegroupstopicjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ChangeGroupsTopicJobTest)
using namespace RocketChatRestApi;
ChangeGroupsTopicJobTest::ChangeGroupsTopicJobTest(QObject *parent)
    : QObject(parent)
{
}

void ChangeGroupsTopicJobTest::shouldHaveDefaultValue()
{
    ChangeGroupsTopicJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.topic().isEmpty());
    QVERIFY(job.roomId().isEmpty());
}

void ChangeGroupsTopicJobTest::shouldGenerateRequest()
{
    ChangeGroupsTopicJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/groups.setTopic")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ChangeGroupsTopicJobTest::shouldGenerateJson()
{
    ChangeGroupsTopicJob job;
    const QString roomId = QStringLiteral("foo1");
    const QString topic = QStringLiteral("topic1");
    job.setRoomId(roomId);
    job.setTopic(topic);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"roomId\":\"%1\",\"topic\":\"%2\"}").arg(roomId).arg(topic).toLatin1());
}
