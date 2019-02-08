/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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


#include "roletest.h"
#include "role.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(RoleTest)

RoleTest::RoleTest(QObject *parent)
    : QObject(parent)
{

}

void RoleTest::shouldHaveDefaultValue()
{
    Role r;
    QVERIFY(!r.isOwner());
    QVERIFY(!r.isModerator());
    QVERIFY(!r.isLeader());
    QVERIFY(r.userId().isEmpty());
    QVERIFY(!r.isValid());
    QVERIFY(!r.hasARole());
}

void RoleTest::shouldBeValid()
{
    Role r;
    QVERIFY(!r.isValid());
    r.setIsOwner(true);
    QVERIFY(!r.isValid());
    r.setIsOwner(false);
    QVERIFY(!r.isValid());
    r.setIsLeader(true);
    QVERIFY(!r.isValid());
    r.setIsLeader(false);
    QVERIFY(!r.isValid());
    r.setIsModerator(true);
    QVERIFY(!r.isValid());
    r.setIsModerator(false);
    QVERIFY(!r.isValid());
    r.setUserId(QStringLiteral("foo"));
    QVERIFY(r.isValid());
    r.setUserId(QString());
    QVERIFY(!r.isValid());    
}


void RoleTest::shouldLoadRoles_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<Role>("role");

    Role r;
    r.setUserId(QStringLiteral("id1"));
    r.setIsOwner(true);
    r.setIsModerator(true);
    r.setIsLeader(true);
    QTest::addRow("role1") << QStringLiteral("role1") << r;
    Role r2;
    r2.setUserId(QStringLiteral("id2"));
    r2.setIsOwner(true);
    r2.setIsLeader(true);
    QTest::addRow("role2") << QStringLiteral("role2") << r2;
}


void RoleTest::shouldLoadRoles()
{
    QFETCH(QString, name);
    QFETCH(Role, role);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/rolesforroom/") + name + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();

    Role r;
    r.parseRole(obj);
    QCOMPARE(r, role);
}

void RoleTest::shouldVerifyHasRoles()
{
    Role r;
    r.setUserId(QStringLiteral("foo"));
    QVERIFY(!r.hasARole());
    r.setIsOwner(true);
    QVERIFY(r.hasARole());
    r.setIsLeader(true);
    QVERIFY(r.hasARole());
    r.setIsModerator(true);
    QVERIFY(r.hasARole());
    r.setIsOwner(false);
    QVERIFY(r.hasARole());
    r.setIsLeader(false);
    QVERIFY(r.hasARole());
    r.setIsModerator(false);
    QVERIFY(!r.hasARole());
}

//Create autotest for updateRoless
