/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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

#include "usertest.h"
#include "ruqola_autotest_helper.h"
#include "user.h"

#include <QTest>
#include <QJsonObject>
#include <QJsonDocument>
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
    QCOMPARE(u.status(), QStringLiteral("offline"));
    QVERIFY(u.userName().isEmpty());
    QVERIFY(!u.isValid());
    QVERIFY(u.roles().isEmpty());
    QVERIFY(!u.userEmailsInfo().isValid());
    QVERIFY(!u.createdAt().isValid());
    QVERIFY(!u.lastLogin().isValid());
    QVERIFY(u.i18nRoles().isEmpty());
}

void UserTest::shouldBeValid()
{
    User u;

    u.setName(QStringLiteral("foo"));
    QVERIFY(u.isValid());

    User u2;
    u2.setUserId(QStringLiteral("bla"));
    QVERIFY(!u2.isValid());

    User u3;
    u3.setUserName(QStringLiteral("dd"));
    QVERIFY(u3.isValid());

    User u4;
    u4.setUserId(QStringLiteral("d"));
    u4.setUserName(QStringLiteral("dd"));
    u4.setName(QStringLiteral("foo"));
    QVERIFY(u4.isValid());
}

void UserTest::shouldSetAndGetName()
{
    User sampleUser;
    QString name = QStringLiteral("Maxwell");
    sampleUser.setName(name);

    QCOMPARE(sampleUser.name(), name);

    name = QStringLiteral("Maxwell_NEW");
    sampleUser.setName(name);
    QCOMPARE(sampleUser.name(), name);

    name = QStringLiteral("Maxwell_NEW");
    sampleUser.setName(name);
    QCOMPARE(sampleUser.name(), name);
}

void UserTest::shouldSetAndGetUserId()
{
    User sampleUser;
    QString Id = QStringLiteral("RA1511ECE");
    sampleUser.setUserId(Id);

    QCOMPARE(sampleUser.userId(), Id);

    Id = QStringLiteral("RA1511ECE_NEW");
    sampleUser.setUserId(Id);
    QCOMPARE(sampleUser.userId(), Id);

    Id = QStringLiteral("RA1511ECE_NEW");
    sampleUser.setUserId(Id);
    QCOMPARE(sampleUser.userId(), Id);
}

void UserTest::shouldSetAndGetStatus()
{
    User sampleUser;
    QString status = QStringLiteral("myStatus");

    sampleUser.setStatus(status);

    QCOMPARE(sampleUser.status(), status);

    status = QStringLiteral("myStatus_NEW");
    sampleUser.setStatus(status);
    QCOMPARE(sampleUser.status(), status);

    status = QStringLiteral("myStatus_NEW");
    sampleUser.setStatus(status);
    QCOMPARE(sampleUser.status(), status);
}

void UserTest::shouldParseUser()
{
    User sampleUser;
    QJsonObject object;
    QJsonObject fields;
    const QString name = QStringLiteral("Newton");
    const QString status = QStringLiteral("Newton");
    fields.insert(QStringLiteral("name"), QJsonValue(name));
    fields.insert(QStringLiteral("status"), QJsonValue(status));
    object.insert(QStringLiteral("id"), QJsonValue(QLatin1String("RA151100ECE")));
    object.insert(QStringLiteral("fields"), fields);

    sampleUser.parseUser(object);
    QCOMPARE(sampleUser.name(), name);
    QCOMPARE(sampleUser.status(), status);

    sampleUser.parseUser(object);
    QCOMPARE(sampleUser.name(), name);
    QCOMPARE(sampleUser.status(), status);
}

void UserTest::checkEqualsAndUnequalsOperator()
{
    User sampleuser, sampleuserOther;
    QString Id = QStringLiteral("RA151100ECE");
    QString name = QStringLiteral("Robert Segwick");
    QString status = QStringLiteral("myStaus");

    sampleuser.setUserId(Id);
    sampleuser.setName(name);
    sampleuser.setStatus(status);
    QVERIFY(sampleuser != sampleuserOther);

    sampleuserOther.setUserId(Id);
    sampleuserOther.setName(name);
    sampleuserOther.setStatus(status);
    QVERIFY(sampleuser == sampleuserOther);

    sampleuserOther.setName(QStringLiteral("Robert Segwick_NEW"));
    QVERIFY(sampleuser != sampleuserOther);
}

void UserTest::shouldParseJson_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<User>("expectedUser");
    User expected;
    expected.setName(QStringLiteral("Laurent M"));
    expected.setStatus(QStringLiteral("away"));
    expected.setUserId(QStringLiteral("yi2ucvqkdkxiTkyZ5"));
    expected.setUserName(QStringLiteral("laurent"));
    expected.setUtcOffset(1);
    QTest::newRow("user1") << QStringLiteral("adduser") << expected;
}

void UserTest::shouldParseJson()
{
    QFETCH(QString, fileName);
    QFETCH(User, expectedUser);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/") + fileName + QLatin1String(".json");
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
        expected.setName(QStringLiteral("name_user"));
        expected.setStatus(QStringLiteral("offline"));
        expected.setUserId(QStringLiteral("BDFj6E7Z9RYucn8C"));
        expected.setUserName(QStringLiteral("username"));
        expected.setUtcOffset(0);
        expected.setRoles({QStringLiteral("user")});
        QDateTime createdTime;
        createdTime.setDate(QDate(2020, 10, 05));
        createdTime.setTime(QTime(00, 48, 01, 903));
        expected.setCreatedAt(createdTime);
        expected.setLastLogin(QDateTime());
        QTest::newRow("userrestapi1") << QStringLiteral("userrestapi") << expected;
    }
    {
        User expected;
        expected.setName(QStringLiteral("Bla bla"));
        expected.setStatus(QStringLiteral("online"));
        expected.setUserId(QStringLiteral("XQZAk3998f9hSNwh"));
        expected.setUserName(QStringLiteral("steffen"));
        expected.setUtcOffset(2);
        expected.setRoles({QStringLiteral("user"), QStringLiteral("admin")});
        QDateTime createdTime;
        createdTime.setDate(QDate(2018, 01, 18));
        createdTime.setTime(QTime(12, 52, 50, 772));
        expected.setCreatedAt(createdTime);
        QDateTime lastLogin;
        lastLogin.setDate(QDate(2020, 10, 12));
        lastLogin.setTime(QTime(02, 26, 27, 324));
        expected.setLastLogin(lastLogin);
        User::UserEmailsInfo info;
        info.email = QStringLiteral("bla@kde.com");
        info.verified = true;
        expected.setUserEmailsInfo(info);
        QTest::newRow("userrestapi2") << QStringLiteral("userrestapi2") << expected;
    }
}

void UserTest::shouldParseRestApiJson()
{
    QFETCH(QString, fileName);
    QFETCH(User, expectedUser);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/json/") + fileName + QLatin1String(".json");
    const QJsonObject fields = AutoTestHelper::loadJsonObject(originalJsonFile);
    User user;
    user.parseUserRestApi(fields);
    const bool equal = (user == expectedUser);
    if (!equal) {
        qDebug() << " current value " << user;
        qDebug() << " expected value " << expectedUser;
    }
    QVERIFY(equal);
}

void UserTest::shouldGetStatusIcon_data()
{
    QTest::addColumn<QString>("status");
    QTest::addColumn<QString>("iconFileName");
    QTest::newRow("online") << QStringLiteral("online") << QStringLiteral("user-online");
    QTest::newRow("busy") << QStringLiteral("busy") << QStringLiteral("user-busy");
    QTest::newRow("away") << QStringLiteral("away") << QStringLiteral("user-away");
    QTest::newRow("offline") << QStringLiteral("offline") << QStringLiteral("user-offline");
    QTest::newRow("unknown") << QStringLiteral("foo") << QStringLiteral("unknown");
    QTest::newRow("unknown1") << QString() << QStringLiteral("unknown");
}

void UserTest::shouldGetStatusIcon()
{
    QFETCH(QString, status);
    QFETCH(QString, iconFileName);
    User user;
    user.setStatus(status);
    QCOMPARE(user.iconFromStatus(), iconFileName);
}
