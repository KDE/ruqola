/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usertest.h"
#include "ruqola_autotest_helper.h"
#include "user.h"

#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(UserTest)

UserTest::UserTest(QObject *parent)
    : QObject(parent)
{
}

void UserTest::shouldHaveDefaultValue()
{
    User u;
    QVERIFY(u.name().isEmpty());
    QVERIFY(u.userId().isEmpty());
    QCOMPARE(u.status(), User::PresenceStatus::Offline);
    QVERIFY(u.userName().isEmpty());
    QVERIFY(!u.isValid());
    QVERIFY(u.roles().isEmpty());
    QVERIFY(!u.userEmailsInfo().isValid());
    QVERIFY(!u.createdAt().isValid());
    QVERIFY(!u.lastLogin().isValid());
    QVERIFY(u.i18nRoles().isEmpty());
    QVERIFY(u.active());
    QVERIFY(u.bio().isEmpty());
    QVERIFY(u.nickName().isEmpty());
}

void UserTest::shouldBeValid()
{
    User u;

    u.setName(u"foo"_s);
    QVERIFY(u.isValid());

    User u2;
    u2.setUserId("bla"_ba);
    QVERIFY(!u2.isValid());

    User u3;
    u3.setUserName(u"dd"_s);
    QVERIFY(u3.isValid());

    User u4;
    u4.setUserId("d"_ba);
    u4.setUserName(u"dd"_s);
    u4.setName(u"foo"_s);
    QVERIFY(u4.isValid());
}

void UserTest::shouldSetAndGetName()
{
    User sampleUser;
    QString name = u"Maxwell"_s;
    sampleUser.setName(name);

    QCOMPARE(sampleUser.name(), name);

    name = u"Maxwell_NEW"_s;
    sampleUser.setName(name);
    QCOMPARE(sampleUser.name(), name);

    name = u"Maxwell_NEW"_s;
    sampleUser.setName(name);
    QCOMPARE(sampleUser.name(), name);
}

void UserTest::shouldSetAndGetUserId()
{
    User sampleUser;
    QByteArray Id = "RA1511ECE"_ba;
    sampleUser.setUserId(Id);

    QCOMPARE(sampleUser.userId(), Id);

    Id = "RA1511ECE_NEW"_ba;
    sampleUser.setUserId(Id);
    QCOMPARE(sampleUser.userId(), Id);

    Id = "RA1511ECE_NEW"_ba;
    sampleUser.setUserId(Id);
    QCOMPARE(sampleUser.userId(), Id);
}

void UserTest::shouldSetAndGetStatus()
{
    User sampleUser;

    sampleUser.setStatus(User::PresenceStatus::Offline);

    QCOMPARE(sampleUser.status(), User::PresenceStatus::Offline);

    sampleUser.setStatus(User::PresenceStatus::Online);
    QCOMPARE(sampleUser.status(), User::PresenceStatus::Online);

    sampleUser.setStatus(User::PresenceStatus::Online);
    QCOMPARE(sampleUser.status(), User::PresenceStatus::Online);
}

void UserTest::shouldParseUser()
{
    User sampleUser;
    QJsonObject object;
    QJsonObject fields;
    const QString name = u"Newton"_s;
    const QString status = u"offline"_s;
    fields.insert(u"name"_s, QJsonValue(name));
    fields.insert(u"status"_s, QJsonValue(status));
    object.insert(u"id"_s, QJsonValue("RA151100ECE"_L1));
    object.insert(u"fields"_s, fields);

    sampleUser.parseUser(object);
    QCOMPARE(sampleUser.name(), name);
    QCOMPARE(sampleUser.status(), User::PresenceStatus::Offline);
}

void UserTest::checkEqualsAndUnequalsOperator()
{
    User sampleuser;
    User sampleuserOther;
    QByteArray Id = "RA151100ECE"_ba;
    QString name = u"Robert Segwick"_s;

    sampleuser.setUserId(Id);
    sampleuser.setName(name);
    sampleuser.setStatus(User::PresenceStatus::Offline);
    QVERIFY(sampleuser != sampleuserOther);

    sampleuserOther.setUserId(Id);
    sampleuserOther.setName(name);
    sampleuserOther.setStatus(User::PresenceStatus::Offline);
    QCOMPARE(sampleuser, sampleuserOther);

    sampleuserOther.setName(u"Robert Segwick_NEW"_s);
    QVERIFY(sampleuser != sampleuserOther);
}

void UserTest::shouldParseJson_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<User>("expectedUser");
    User expected;
    expected.setName(u"Laurent M"_s);
    expected.setStatus(User::PresenceStatus::Away);
    expected.setUserId("yi2ucvqkdkxiTkyZ5"_ba);
    expected.setUserName(u"laurent"_s);
    expected.setUtcOffset(1);
    QTest::newRow("user1") << u"adduser"_s << expected;
}

void UserTest::shouldParseJson()
{
    QFETCH(QString, fileName);
    QFETCH(User, expectedUser);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/"_L1 + fileName + ".json"_L1;
    const QJsonObject fields = AutoTestHelper::loadJsonObject(originalJsonFile);
    User user;
    user.parseUser(fields);
    const bool equal = (user == expectedUser);
    if (!equal) {
        qDebug() << " current value " << user;
        qDebug() << " expected value " << expectedUser;
    }
    QVERIFY(equal);
}

void UserTest::shouldParseRestApiJson_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<User>("expectedUser");
    {
        User expected;
        expected.setName(u"name_user"_s);
        expected.setStatus(User::PresenceStatus::Offline);
        expected.setUserId("BDFj6E7Z9RYucn8C"_ba);
        expected.setUserName(u"username"_s);
        expected.setUtcOffset(0);
        expected.setRoles({u"user"_s}, {});
        expected.setType("user"_L1);
        QDateTime createdTime;
        createdTime.setDate(QDate(2020, 10, 04));
        createdTime.setTime(QTime(22, 48, 01, 903));
        createdTime.setTimeZone(QTimeZone::UTC);
        expected.setCreatedAt(createdTime);
        expected.setLastLogin(QDateTime());
        QTest::newRow("userrestapi1") << u"userrestapi"_s << expected;
    }
    {
        User expected;
        expected.setName(u"Bla bla"_s);
        expected.setStatus(User::PresenceStatus::Online);
        expected.setUserId("XQZAk3998f9hSNwh"_ba);
        expected.setUserName(u"steffen"_s);
        expected.setUtcOffset(2);
        expected.setType("user"_L1);
        expected.setRoles({u"user"_s, u"admin"_s}, {});
        QDateTime createdTime;
        createdTime.setTimeZone(QTimeZone::UTC);
        createdTime.setDate(QDate(2018, 01, 18));
        createdTime.setTime(QTime(11, 52, 50, 772));

        expected.setCreatedAt(createdTime);
        QDateTime lastLogin;
        lastLogin.setTimeZone(QTimeZone::UTC);
        lastLogin.setDate(QDate(2020, 10, 12));
        lastLogin.setTime(QTime(00, 26, 27, 324));
        expected.setLastLogin(lastLogin);
        User::UserEmailsInfo info;
        info.email = u"bla@kde.com"_s;
        info.verified = true;
        expected.setUserEmailsInfo(info);
        expected.setNickName(u"TheReal"_s);
        QTest::newRow("userrestapi2") << u"userrestapi2"_s << expected;
    }
}

void UserTest::shouldParseRestApiJson()
{
    QFETCH(QString, fileName);
    QFETCH(User, expectedUser);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/json/"_L1 + fileName + ".json"_L1;
    const QJsonObject fields = AutoTestHelper::loadJsonObject(originalJsonFile);
    User user;
    user.parseUserRestApi(fields, {});
    const bool equal = (user == expectedUser);
    if (!equal) {
        qDebug() << " current value " << user;
        qDebug() << " expected value " << expectedUser;
    }
    QVERIFY(equal);
}

void UserTest::shouldGetStatusIcon_data()
{
    QTest::addColumn<User::PresenceStatus>("status");
    QTest::addColumn<QString>("iconFileName");
    QTest::newRow("online") << User::PresenceStatus::Online << u"user-online"_s;
    QTest::newRow("busy") << User::PresenceStatus::Busy << u"user-busy"_s;
    QTest::newRow("away") << User::PresenceStatus::Away << u"user-away"_s;
    QTest::newRow("offline") << User::PresenceStatus::Offline << u"user-offline"_s;
    QTest::newRow("unknown") << User::PresenceStatus::Unknown << u"unknown"_s;
}

void UserTest::shouldGetStatusIcon()
{
    QFETCH(User::PresenceStatus, status);
    QFETCH(QString, iconFileName);
    User user;
    user.setStatus(status);
    QCOMPARE(user.iconFromStatus(), iconFileName);
}

#include "moc_usertest.cpp"
