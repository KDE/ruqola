/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rolestest.h"
#include "roles/roles.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(RolesTest)

RolesTest::RolesTest(QObject *parent)
    : QObject(parent)
{
}

void RolesTest::shouldHaveDefaultValue()
{
    const Roles r;
    QVERIFY(r.roles().isEmpty());
    QVERIFY(r.isEmpty());
}

void RolesTest::shouldFindRoles()
{
    Roles r;

    QList<Role> v;

    Role r1;
    r1.setIsOwner(true);
    r1.setIsLeader(true);
    r1.setUserId("r1"_ba);
    v.append(std::move(r1));

    Role r2;
    r2.setIsOwner(true);
    r2.setIsLeader(false);
    r2.setUserId("r2"_ba);
    v.append(std::move(r2));

    r.setRoles(v);

    Role result = r.findRoleByUserId("r1"_ba);
    QVERIFY(result.isValid());
    QVERIFY(result.isOwner());
    QVERIFY(result.isLeader());
    QVERIFY(!result.isModerator());

    result = r.findRoleByUserId("r7"_ba);
    QVERIFY(!result.isValid());
}

void RolesTest::shouldLoadRoles_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("rolesCount");

    QTest::addRow("roles1") << u"roles1"_s << 3;
}

void RolesTest::shouldLoadRoles()
{
    QFETCH(QString, name);
    QFETCH(int, rolesCount);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/rolesforroom/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    Roles r;
    r.parseRole(obj);
    QCOMPARE(r.roles().count(), rolesCount);
}

void RolesTest::shouldUpdateRoles()
{
    Roles r;
    QJsonObject user;
    user["_id"_L1] = u"id1"_s;
    user["username"_L1] = u"bla"_s;

    QJsonObject added;
    added["type"_L1] = u"added"_s;
    added["_id"_L1] = u"owner"_s;
    added["u"_L1] = user;
    r.updateRoles(added);

    QCOMPARE(r.count(), 1);
    QVERIFY(r.at(0).isOwner());
    // The username must be stored too, otherwise the channel role dialog shows empty names.
    QCOMPARE(r.at(0).userName(), u"bla"_s);

    QJsonObject added2 = added;
    added2["_id"_L1] = u"moderator"_s;
    r.updateRoles(added2);
    QCOMPARE(r.count(), 1);
    QVERIFY(r.at(0).isOwner());
    QVERIFY(r.at(0).isModerator());

    QJsonObject removed = added;
    removed["type"_L1] = u"removed"_s;
    r.updateRoles(removed);
    QCOMPARE(r.count(), 1);
    QVERIFY(!r.at(0).isOwner());
    QVERIFY(r.at(0).isModerator());
    QCOMPARE(r.at(0).userName(), u"bla"_s);

    QJsonObject removed2 = added2;
    removed2["type"_L1] = u"removed"_s;
    r.updateRoles(removed2);
    QCOMPARE(r.count(), 0);
}

#include "moc_rolestest.cpp"
