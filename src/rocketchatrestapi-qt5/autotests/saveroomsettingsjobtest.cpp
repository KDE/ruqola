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

#include "saveroomsettingsjobtest.h"
#include "rooms/saveroomsettingsjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(SaveRoomSettingsJobTest)
using namespace RocketChatRestApi;
SaveRoomSettingsJobTest::SaveRoomSettingsJobTest(QObject *parent)
    : QObject(parent)
{
}

void SaveRoomSettingsJobTest::shouldHaveDefaultValue()
{
    SaveRoomSettingsJob job;
    verifyDefaultValue(&job);
    QVERIFY(!job.saveRoomSettingsInfo().isValid());

    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.saveRoomSettingsInfo().isValid());
    QVERIFY(!job.hasQueryParameterSupport());
}

void SaveRoomSettingsJobTest::shouldGenerateRequest()
{
    SaveRoomSettingsJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/rooms.saveRoomSettings")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void SaveRoomSettingsJobTest::shouldGenerateJson()
{
    SaveRoomSettingsJob job;
    const QString roomId = QStringLiteral("foo1");
    SaveRoomSettingsJob::SaveRoomSettingsInfo info;
    info.roomId = roomId;
    job.setSaveRoomSettingsInfo(info);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"rid":"%1"})").arg(roomId).toLatin1());

    const QString roomAnnouncement = QStringLiteral("announcement");
    info.mSettingsWillBeChanged |= SaveRoomSettingsJob::SaveRoomSettingsInfo::RoomAnnouncement;
    info.roomAnnouncement = roomAnnouncement;
    job.setSaveRoomSettingsInfo(info);
    const QStringList systemMessages{QStringLiteral("ua"), QStringLiteral("ca")};
    info.mSettingsWillBeChanged |= SaveRoomSettingsJob::SaveRoomSettingsInfo::SystemMessages;
    info.systemMessages = systemMessages;
    job.setSaveRoomSettingsInfo(info);
    QString res;
    for (const QString &s : systemMessages) {
        if (!res.isEmpty()) {
            res += QLatin1Char(',');
        }
        res += QStringLiteral("\"%1\"").arg(s);
    }
    QCOMPARE(job.json().toJson(QJsonDocument::Compact),
             QStringLiteral(R"({"rid":"%1","roomAnnouncement":"%2","systemMessages":[%3]})").arg(roomId, roomAnnouncement, res).toLatin1());
}

void SaveRoomSettingsJobTest::shouldNotStarting()
{
    SaveRoomSettingsJob job;

    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString roomId = QStringLiteral("foo1");
    SaveRoomSettingsJob::SaveRoomSettingsInfo info;
    info.roomId = roomId;
    job.setSaveRoomSettingsInfo(info);
    QVERIFY(!job.canStart());

    // We need to change a settings
    //     QVERIFY(job.canStart());
}
