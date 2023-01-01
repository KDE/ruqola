/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rolestest.h"
#include "roles/roles.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
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

    QVector<Role> v;

    Role r1;
    r1.setIsOwner(true);
    r1.setIsLeader(true);
    r1.setUserId(QStringLiteral("r1"));
    v.append(std::move(r1));

    Role r2;
    r2.setIsOwner(true);
    r2.setIsLeader(false);
    r2.setUserId(QStringLiteral("r2"));
    v.append(std::move(r2));

    r.setRoles(v);

    Role result = r.findRoleByUserId(QStringLiteral("r1"));
    QVERIFY(result.isValid());
    QVERIFY(result.isOwner());
    QVERIFY(result.isLeader());
    QVERIFY(!result.isModerator());

    result = r.findRoleByUserId(QStringLiteral("r7"));
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
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/rolesforroom/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    Roles r;
    r.parseRole(obj);
    QCOMPARE(r.roles().count(), rolesCount);
}
