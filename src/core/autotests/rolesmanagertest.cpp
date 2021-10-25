/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "rolesmanagertest.h"
#include "roles/rolesmanager.h"
#include "ruqola_autotest_helper.h"
#include <QJsonArray>
#include <QTest>
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
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/rolesmanager/") + name + QLatin1String(".json");
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
    {
        // Add new element
        QTest::addRow("add-element") << QStringLiteral("initialstate1") << QStringLiteral("update1") << 12 << 13;
    }
    {
        // Update an element
        QTest::addRow("update-element") << QStringLiteral("initialstate1") << QStringLiteral("update2") << 12 << 12;
    }
    {
        // Remove an element
        // QTest::addRow("remove-element") << QStringLiteral("initialstate1") << QStringLiteral("update1") << 12;
    }
}

void RolesManagerTest::shouldUpdateRoles()
{
    QFETCH(QString, name);
    QFETCH(QString, updateName);
    QFETCH(int, numberOfRolesBefore);
    QFETCH(int, numberOfRoles);

    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/rolesmanager/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    RolesManager m;
    m.parseRoles(obj);
    QCOMPARE(m.roleInfo().count(), numberOfRolesBefore);

    const QString updateJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/rolesmanager/") + updateName + QLatin1String(".json");
    const QJsonArray array = AutoTestHelper::loadJsonArrayObject(updateJsonFile);
    m.updateRoles(array);
    QCOMPARE(m.roleInfo().count(), numberOfRoles);

    // TODO
}
