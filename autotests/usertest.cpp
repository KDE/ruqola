/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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
#include "user.h"

#include <QTest>
#include <QSignalSpy>
#include <QJsonObject>
#include <QJsonDocument>
QTEST_GUILESS_MAIN(UserTest)

//TODO test username too

UserTest::UserTest(QObject *parent)
    : QObject(parent)
{
}

void UserTest::shouldHaveDefaultValue()
{
    User u;
    QVERIFY(u.name().isEmpty());
    QVERIFY(u.userId().isEmpty());
    QVERIFY(u.status().isEmpty());
    QVERIFY(u.userName().isEmpty());
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
    QTest::newRow("user1") << QStringLiteral("adduser") << expected;
}

void UserTest::shouldParseJson()
{
    QFETCH(QString, fileName);
    QFETCH(User, expectedUser);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/json/") + fileName + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object();

    User user;
    user.parseUser(fields);
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
