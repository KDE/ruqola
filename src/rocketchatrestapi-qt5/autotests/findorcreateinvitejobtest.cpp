/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "findorcreateinvitejobtest.h"
#include "invite/findorcreateinvitejob.h"
#include "ruqola_restapi_helper.h"
#include <QTest>
#include <QJsonDocument>
#include <QJsonObject>
QTEST_GUILESS_MAIN(FindOrCreateInviteJobTest)
using namespace RocketChatRestApi;
FindOrCreateInviteJobTest::FindOrCreateInviteJobTest(QObject *parent)
    : QObject(parent)
{
}

void FindOrCreateInviteJobTest::shouldHaveDefaultValue()
{
    FindOrCreateInviteJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void FindOrCreateInviteJobTest::shouldGenerateRequest()
{
    FindOrCreateInviteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/findOrCreateInvite")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void FindOrCreateInviteJobTest::shouldGenerateJson()
{
    FindOrCreateInviteJob job;

    const QString roomId = QStringLiteral("foo1");
    job.setRoomId(roomId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"days\":0,\"maxUses\":20,\"rid\":\"%1\"}").arg(roomId).toLatin1());
    job.setMaxUses(32);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"days\":0,\"maxUses\":32,\"rid\":\"%1\"}").arg(roomId).toLatin1());
    job.setNumberOfDays(42);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral("{\"days\":42,\"maxUses\":32,\"rid\":\"%1\"}").arg(roomId).toLatin1());
}

void FindOrCreateInviteJobTest::shouldNotStarting()
{
    FindOrCreateInviteJob job;

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
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

void FindOrCreateInviteJobTest::shouldParseResult_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<FindOrCreateInviteJob::InviteUsersInfo>("inviteUsersInfo");
    FindOrCreateInviteJob::InviteUsersInfo info;
    info.url = QUrl(QStringLiteral("https://go.rocket.chat/invite?host=www.kde.org&path=invite%2FPE6Nq6"));
    info.maxUses = 0;
    info.roomId = QStringLiteral("QMkvkiMyxKoEuJjnb");
    info.userId = QStringLiteral("bla");

    QTest::newRow("test1") << QStringLiteral("test1") << info;
}

void FindOrCreateInviteJobTest::shouldParseResult()
{
    QFETCH(QString, name);
    QFETCH(FindOrCreateInviteJob::InviteUsersInfo, inviteUsersInfo);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/inviteusers/") + name + QLatin1String(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    QJsonObject obj = doc.object();
    f.close();
    FindOrCreateInviteJob job;
    const FindOrCreateInviteJob::InviteUsersInfo result = job.parseResult(obj);
    const bool messageIsEqual = (result == inviteUsersInfo);
    if (!messageIsEqual) {
        qDebug() << "originalMessage "<<result;
        qDebug() << "ExpectedMessage "<<inviteUsersInfo;
    }
    QVERIFY(messageIsEqual);
}
