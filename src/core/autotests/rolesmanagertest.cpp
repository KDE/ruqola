/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rolesmanagertest.h"
#include "roles/rolesmanager.h"
#include "ruqola_autotest_helper.h"
#include <QJsonArray>
#include <QSignalSpy>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(RolesManagerTest)
RolesManagerTest::RolesManagerTest(QObject *parent)
    : QObject{parent}
{
}

void RolesManagerTest::shouldHaveDefaultValues()
{
    RolesManager m;
    QVERIFY(m.roleInfo().isEmpty());
}

void RolesManagerTest::shouldLoadRoles_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("numberOfRoles");
    {
        QTest::addRow("test1") << QStringLiteral("test1") << 12;
    }
}

void RolesManagerTest::shouldLoadRoles()
{
    QFETCH(QString, name);
    QFETCH(int, numberOfRoles);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/rolesmanager/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    RolesManager m;
    m.parseRoles(obj);
    QCOMPARE(m.roleInfo().count(), numberOfRoles);
}

void RolesManagerTest::shouldUpdateRoles_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("updateName");
    QTest::addColumn<int>("numberOfRolesBefore");
    QTest::addColumn<int>("numberOfRoles");
    QTest::addColumn<int>("numberOfEmitSignal");
    {
        // Add new element
        QTest::addRow("add-element") << QStringLiteral("initialstate1") << QStringLiteral("update1") << 12 << 13 << 1;
    }
    {
        // Update an element
        QTest::addRow("update-element") << QStringLiteral("initialstate1") << QStringLiteral("update2") << 12 << 12 << 1;
    }
    {
        // Remove an element
        QTest::addRow("remove-element") << QStringLiteral("initialstate1") << QStringLiteral("remove1") << 12 << 11 << 1;
    }
    {
        // Invalid remove element
        QTest::addRow("remove-element-not-existing") << QStringLiteral("initialstate1") << QStringLiteral("remove-not-existing") << 12 << 12 << 0;
    }
}

void RolesManagerTest::shouldUpdateRoles()
{
    QFETCH(QString, name);
    QFETCH(QString, updateName);
    QFETCH(int, numberOfRolesBefore);
    QFETCH(int, numberOfRoles);
    QFETCH(int, numberOfEmitSignal);

    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/rolesmanager/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    RolesManager m;
    QSignalSpy spy(&m, &RolesManager::rolesChanged);
    m.parseRoles(obj);
    QCOMPARE(m.roleInfo().count(), numberOfRolesBefore);

    const QString updateJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/rolesmanager/"_L1 + updateName + ".json"_L1;
    const QJsonArray array = AutoTestHelper::loadJsonArrayObject(updateJsonFile);
    m.updateRoles(array);
    QCOMPARE(m.roleInfo().count(), numberOfRoles);

    QCOMPARE(spy.count(), numberOfEmitSignal);
}

#include "moc_rolesmanagertest.cpp"
