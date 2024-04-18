/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rolestest.h"
#include "roles/roles.h"
#include "ruqola_autotest_helper.h"
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(RolesTest)

RolesTest::RolesTest(QObject *parent)
    : QObject(parent)
{
}

void RolesTest::shouldHaveDefaultValue()
{
    Roles r;
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
    r1.setUserId(QByteArrayLiteral("r1"));
    v.append(std::move(r1));

    Role r2;
    r2.setIsOwner(true);
    r2.setIsLeader(false);
    r2.setUserId(QByteArrayLiteral("r2"));
    v.append(std::move(r2));

    r.setRoles(v);

    Role result = r.findRoleByUserId(QByteArrayLiteral("r1"));
    QVERIFY(result.isValid());
    QVERIFY(result.isOwner());
    QVERIFY(result.isLeader());
    QVERIFY(!result.isModerator());

    result = r.findRoleByUserId(QByteArrayLiteral("r7"));
    QVERIFY(!result.isValid());
}

void RolesTest::shouldLoadRoles_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("rolesCount");

    QTest::addRow("roles1") << QStringLiteral("roles1") << 3;
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

#include "moc_rolestest.cpp"
