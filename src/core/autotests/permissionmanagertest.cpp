/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionmanagertest.h"
#include "permissions/permissionmanager.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(PermissionManagerTest)
PermissionManagerTest::PermissionManagerTest(QObject *parent)
    : QObject(parent)
{
}

void PermissionManagerTest::shouldHaveDefaultValues()
{
    PermissionManager w;
    QCOMPARE(w.permissionCount(), 0);
}

void PermissionManagerTest::shouldHasRoles()
{
    PermissionManager w;
    QVERIFY(w.roles(QStringLiteral("bla")).isEmpty());
}

void PermissionManagerTest::shouldLoadPermissions_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("permissionsCount");
    QTest::addColumn<int>("permissionsAdded");
    QTest::addColumn<QVector<Permission>>("permissions");

    {
        QVector<Permission> permissions;

        {
            Permission p;
            p.setIdentifier(QStringLiteral("access-permissions"));
            p.setRoles({QStringLiteral("admin")});
            p.setUpdatedAt(1533550243852);
            permissions.append(std::move(p));
        }
        {
            Permission p;
            p.setIdentifier(QStringLiteral("add-user-to-joined-room"));
            p.setRoles({QStringLiteral("admin"), QStringLiteral("owner"), QStringLiteral("moderator")});
            p.setUpdatedAt(1533550243869);
            permissions.append(std::move(p));
        }
        {
            Permission p;
            p.setIdentifier(QStringLiteral("add-user-to-any-c-room"));
            p.setRoles({QStringLiteral("admin")});
            p.setUpdatedAt(1533550243881);
            permissions.append(std::move(p));
        }
        {
            Permission p;
            p.setIdentifier(QStringLiteral("add-user-to-any-p-room"));
            p.setUpdatedAt(1533550243889);
            permissions.append(std::move(p));
        }
        QTest::addRow("permissions1") << QStringLiteral("permissions1") << 5 << 4 << permissions;
    }
}

void PermissionManagerTest::shouldLoadPermissions()
{
    QFETCH(QString, name);
    QFETCH(int, permissionsCount);
    QFETCH(int, permissionsAdded);
    QFETCH(QVector<Permission>, permissions);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/permissions/") + name + QLatin1String(".json");
    const QJsonArray obj = AutoTestHelper::loadJsonArrayObject(originalJsonFile);
    QCOMPARE(obj.count(), permissionsCount);
    // qDebug() << " OBJ " << obj;

    PermissionManager r;
    r.parseUpdatePermission(obj);
    QCOMPARE(r.permissionCount(), permissionsAdded);
    QCOMPARE(r.permissionCount(), permissions.count());
    for (const Permission &p : permissions) {
        const Permission managerPermission = r.permission(p.identifier());
        const bool equalPermission = (managerPermission == p);
        if (!equalPermission) {
            qDebug() << p;
            qDebug() << managerPermission;
        }
        QVERIFY(equalPermission);
    }
}

void PermissionManagerTest::shouldUpdatePermissions_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("updateName");
    QTest::addColumn<QVector<Permission>>("permissions");
    QTest::addColumn<QVector<Permission>>("updatedPermissions");
    QTest::addColumn<bool>("permissionUpdated");
    {
        // No updated permission as this permission can't be store in manager
        QVector<Permission> permissions;
        {
            Permission p;
            p.setIdentifier(QStringLiteral("access-permissions"));
            p.setRoles({QStringLiteral("admin")});
            p.setUpdatedAt(1533550243852);
            permissions.append(std::move(p));
        }
        {
            Permission p;
            p.setIdentifier(QStringLiteral("add-user-to-joined-room"));
            p.setRoles({QStringLiteral("admin"), QStringLiteral("owner"), QStringLiteral("moderator")});
            p.setUpdatedAt(1533550243869);
            permissions.append(std::move(p));
        }
        {
            Permission p;
            p.setIdentifier(QStringLiteral("add-user-to-any-c-room"));
            p.setRoles({QStringLiteral("admin")});
            p.setUpdatedAt(1533550243881);
            permissions.append(std::move(p));
        }
        {
            Permission p;
            p.setIdentifier(QStringLiteral("add-user-to-any-p-room"));
            p.setUpdatedAt(1533550243889);
            permissions.append(std::move(p));
        }
        QTest::addRow("permissions1") << QStringLiteral("permissions1") << QStringLiteral("update-permissions1") << permissions << permissions << false;
    }
    {
        // No updated permission as this permission can't be store in manager
        QVector<Permission> permissions;
        QVector<Permission> updatedPermissions;
        {
            Permission p;
            p.setIdentifier(QStringLiteral("access-permissions"));
            p.setRoles({QStringLiteral("admin")});
            p.setUpdatedAt(1533550243852);
            permissions.append(p);
            updatedPermissions.append(p);
        }
        {
            Permission p;
            p.setIdentifier(QStringLiteral("add-user-to-joined-room"));
            p.setRoles({QStringLiteral("admin"), QStringLiteral("owner"), QStringLiteral("moderator")});
            p.setUpdatedAt(1533550243869);
            permissions.append(p);
            updatedPermissions.append(p);
        }
        {
            Permission p;
            p.setIdentifier(QStringLiteral("add-user-to-any-c-room"));
            p.setRoles({QStringLiteral("admin")});
            p.setUpdatedAt(1533550243881);
            permissions.append(p);

            p.setRoles({QStringLiteral("admin"), QStringLiteral("vFXCWG9trXLti6xQm")});
            p.setUpdatedAt(1634569746270);
            updatedPermissions.append(p);
        }
        {
            Permission p;
            p.setIdentifier(QStringLiteral("add-user-to-any-p-room"));
            p.setUpdatedAt(1533550243889);
            permissions.append(p);
            updatedPermissions.append(p);
        }
        // Use permissions1 as ref.
        QTest::addRow("permissions2") << QStringLiteral("permissions1") << QStringLiteral("update-permissions2") << permissions << updatedPermissions << true;
    }
}

void PermissionManagerTest::shouldUpdatePermissions()
{
    QFETCH(QString, name);
    QFETCH(QString, updateName);
    QFETCH(QVector<Permission>, permissions);
    QFETCH(QVector<Permission>, updatedPermissions);
    QFETCH(bool, permissionUpdated);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/permissions/") + name + QLatin1String(".json");
    const QJsonArray obj = AutoTestHelper::loadJsonArrayObject(originalJsonFile);

    const QString originalUpdateJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/permissions/") + updateName + QLatin1String(".json");
    const QJsonArray updateArray = AutoTestHelper::loadJsonArrayObject(originalUpdateJsonFile);

    PermissionManager r;
    r.parseUpdatePermission(obj);
    QCOMPARE(r.permissionCount(), permissions.count());

    for (const Permission &p : permissions) {
        const Permission managerPermission = r.permission(p.identifier());
        const bool equalPermission = (managerPermission == p);
        if (!equalPermission) {
            qDebug() << p;
            qDebug() << managerPermission;
        }
        QVERIFY(equalPermission);
    }

    QCOMPARE(r.updatePermission(updateArray), permissionUpdated);

    for (const Permission &p : updatedPermissions) {
        const Permission managerPermission = r.permission(p.identifier());
        const bool equalPermission = (managerPermission == p);
        if (!equalPermission) {
            qDebug() << p;
            qDebug() << managerPermission;
        }
        QVERIFY(equalPermission);
    }
}
