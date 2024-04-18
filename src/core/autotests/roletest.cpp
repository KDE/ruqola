/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roletest.h"
#include "roles/role.h"
#include "ruqola_autotest_helper.h"
using namespace Qt::Literals::StringLiterals;
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
    QVERIFY(r.userName().isEmpty());
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
    r.setUserId(QByteArrayLiteral("foo"));
    QVERIFY(r.isValid());
    r.setUserId(QByteArray());
    QVERIFY(!r.isValid());
}

void RoleTest::shouldLoadRoles_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<Role>("role");

    Role r;
    r.setUserId(QByteArrayLiteral("id1"));
    r.setIsOwner(true);
    r.setIsModerator(true);
    r.setIsLeader(true);
    r.setUserName(QStringLiteral("bla"));
    QTest::addRow("role1") << QStringLiteral("role1") << r;
    Role r2;
    r2.setUserId(QByteArrayLiteral("id2"));
    r2.setIsOwner(true);
    r2.setIsLeader(true);
    r2.setUserName(QStringLiteral("bla"));
    QTest::addRow("role2") << QStringLiteral("role2") << r2;
}

void RoleTest::shouldLoadRoles()
{
    QFETCH(QString, name);
    QFETCH(Role, role);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/rolesforroom/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    Role r;
    r.parseRole(obj);
    QCOMPARE(r, role);
}

void RoleTest::shouldVerifyHasRoles()
{
    Role r;
    r.setUserId(QByteArrayLiteral("foo"));
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

// Create autotest for updateRoless

#include "moc_roletest.cpp"
