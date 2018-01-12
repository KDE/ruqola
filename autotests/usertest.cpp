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
QTEST_MAIN(UserTest)

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
    QVERIFY(u.listRooms().isEmpty());
}

void UserTest::shouldSetAndGetName()
{
    User sampleUser;
    QSignalSpy spy(&sampleUser, &User::nameChanged);
    QString name = QStringLiteral("Maxwell");
    sampleUser.setName(name);

    QCOMPARE(sampleUser.name(), name);
    QCOMPARE(spy.count(), 1);

    name = QStringLiteral("Maxwell_NEW");
    sampleUser.setName(name);
    QCOMPARE(sampleUser.name(), name);
    QCOMPARE(spy.count(), 2);

    name = QStringLiteral("Maxwell_NEW");
    sampleUser.setName(name);
    QCOMPARE(sampleUser.name(), name);
    QCOMPARE(spy.count(), 2);
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

    QSignalSpy spy(&sampleUser, &User::statusChanged);
    sampleUser.setStatus(status);

    QCOMPARE(sampleUser.status(), status);
    QCOMPARE(spy.count(), 1);

    status = QStringLiteral("myStatus_NEW");
    sampleUser.setStatus(status);
    QCOMPARE(sampleUser.status(), status);
    QCOMPARE(spy.count(), 2);

    status = QStringLiteral("myStatus_NEW");
    sampleUser.setStatus(status);
    QCOMPARE(sampleUser.status(), status);
    QCOMPARE(spy.count(), 2);
}


void UserTest::shouldSetAndGetListRooms()
{
    User sampleUser;
    QStringList roomList;
    QString room1 = QStringLiteral("myRoom1");
    QString room2 = QStringLiteral("myRoom2");
    QString room3 = QStringLiteral("myRoom3");
    roomList.append(room1);
    roomList.append(room2);
    roomList.append(room3);

    QSignalSpy spy(&sampleUser, &User::listRoomsChanged);
    sampleUser.setListRooms(roomList);
    QCOMPARE(sampleUser.listRooms(), roomList);
    QCOMPARE(spy.count(), 1);
}


void UserTest::shouldParseUser()
{
    User sampleUser;
    QJsonObject object;
    QJsonObject fields;
    const QString name = QStringLiteral("Newton");
    const QString status = QStringLiteral("Newton");
    fields.insert(QLatin1String("name"), QJsonValue(name));
    fields.insert(QLatin1String("status"), QJsonValue(status));
    object.insert(QLatin1String("id"), QJsonValue(QLatin1String("RA151100ECE")));
    object.insert(QLatin1String("fields"), fields);

    QSignalSpy spyN(&sampleUser, &User::nameChanged);
    QSignalSpy spyS(&sampleUser, &User::statusChanged);
    sampleUser.parseUser(object);
    QCOMPARE(sampleUser.name(), name);
    QCOMPARE(sampleUser.status(), status);
    QCOMPARE(spyN.count(), 1);
    QCOMPARE(spyS.count(), 1);

    //here signal should not be emmited since we are passing same values
    sampleUser.parseUser(object);
    QCOMPARE(spyN.count(), 1);
    QCOMPARE(spyS.count(), 1);
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

    sampleuserOther.setName(QLatin1String("Robert Segwick_NEW"));
    QVERIFY(sampleuser != sampleuserOther);
}
