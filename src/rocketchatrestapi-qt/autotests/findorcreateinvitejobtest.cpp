/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "findorcreateinvitejobtest.h"

#include "invite/findorcreateinvitejob.h"
#include "restapimethod.h"
#include "ruqola_restapi_helper.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QTest>

using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(FindOrCreateInviteJobTest)
using namespace RocketChatRestApi;
FindOrCreateInviteJobTest::FindOrCreateInviteJobTest(QObject *parent)
    : QObject(parent)
{
}

void FindOrCreateInviteJobTest::shouldHaveDefaultValue()
{
    FindOrCreateInviteJob job;
    RuqolaRestApiHelper::verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(!job.hasQueryParameterSupport());
}

void FindOrCreateInviteJobTest::shouldGenerateRequest()
{
    FindOrCreateInviteJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    RuqolaRestApiHelper::verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(u"http://www.kde.org/api/v1/findOrCreateInvite"_s));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), u"application/json"_s);
}

void FindOrCreateInviteJobTest::shouldGenerateJson()
{
    FindOrCreateInviteJob job;

    const QByteArray roomId("foo1");
    job.setRoomId(roomId);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"days":0,"maxUses":20,"rid":"%1"})").arg(QLatin1StringView(roomId)).toLatin1());
    job.setMaxUses(32);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"days":0,"maxUses":32,"rid":"%1"})").arg(QLatin1StringView(roomId)).toLatin1());
    job.setNumberOfDays(42);
    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"days":42,"maxUses":32,"rid":"%1"})").arg(QLatin1StringView(roomId)).toLatin1());
}

void FindOrCreateInviteJobTest::shouldNotStarting()
{
    FindOrCreateInviteJob job;

    RestApiMethod method;
    method.setServerUrl(u"http://www.kde.org"_s);
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = u"foo"_s;
    const QString userId = u"foo"_s;
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QByteArray roomId("foo1");
    job.setRoomId(roomId);
    QVERIFY(job.canStart());
}

void FindOrCreateInviteJobTest::shouldParseResult_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<FindOrCreateInviteJob::InviteUsersInfo>("inviteUsersInfo");
    FindOrCreateInviteJob::InviteUsersInfo info;
    info.url = QUrl(u"https://go.rocket.chat/invite?host=www.kde.org&path=invite%2FPE6Nq6"_s);
    info.maxUses = 0;
    info.roomId = u"QMkvkiMyxKoEuJjnb"_s;
    info.userId = u"bla"_s;

    QTest::newRow("test1") << u"test1"_s << info;
}

void FindOrCreateInviteJobTest::shouldParseResult()
{
    QFETCH(QString, name);
    QFETCH(FindOrCreateInviteJob::InviteUsersInfo, inviteUsersInfo);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/inviteusers/"_L1 + name + ".json"_L1;
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
        qDebug() << "originalMessage " << result;
        qDebug() << "ExpectedMessage " << inviteUsersInfo;
    }
    QVERIFY(messageIsEqual);
}

#include "moc_findorcreateinvitejobtest.cpp"
