/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionsmanagertest.h"
#include "permissions/permissionsmanager.h"
#include "ruqola_autotest_helper.h"
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(PermissionsManagerTest)
PermissionsManagerTest::PermissionsManagerTest(QObject *parent)
    : QObject(parent)
{
}

void PermissionsManagerTest::shouldHaveDefaultValues()
{
    PermissionsManager w;
    QCOMPARE(w.permissionCount(), 0);
}

void PermissionsManagerTest::shouldHasRoles()
{
    PermissionsManager w;
    QVERIFY(w.roles(QStringLiteral("bla")).isEmpty());
}

void PermissionsManagerTest::shouldLoadPermissions_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("permissionsCount");
    QTest::addColumn<int>("permissionsAdded");
    QTest::addColumn<QList<Permission>>("permissions");

    {
        QList<Permission> permissions;

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

void PermissionsManagerTest::shouldLoadPermissions()
{
    QFETCH(QString, name);
    QFETCH(int, permissionsCount);
    QFETCH(int, permissionsAdded);
    QFETCH(QList<Permission>, permissions);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/permissions/"_L1 + name + ".json"_L1;
    const QJsonArray obj = AutoTestHelper::loadJsonArrayObject(originalJsonFile);
    QCOMPARE(obj.count(), permissionsCount);
    // qDebug() << " OBJ " << obj;

    PermissionsManager r;
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

void PermissionsManagerTest::shouldUpdatePermissions_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("updateName");
    QTest::addColumn<QList<Permission>>("permissions");
    QTest::addColumn<QList<Permission>>("updatedPermissions");
    QTest::addColumn<bool>("permissionUpdated");
    {
        // No updated permission as this permission can't be store in manager
        QList<Permission> permissions;
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
        QList<Permission> permissions;
        QList<Permission> updatedPermissions;
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

void PermissionsManagerTest::shouldUpdatePermissions()
{
    QFETCH(QString, name);
    QFETCH(QString, updateName);
    QFETCH(QList<Permission>, permissions);
    QFETCH(QList<Permission>, updatedPermissions);
    QFETCH(bool, permissionUpdated);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/permissions/"_L1 + name + ".json"_L1;
    const QJsonArray obj = AutoTestHelper::loadJsonArrayObject(originalJsonFile);

    const QString originalUpdateJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/permissions/"_L1 + updateName + ".json"_L1;
    const QJsonArray updateArray = AutoTestHelper::loadJsonArrayObject(originalUpdateJsonFile);

    PermissionsManager r;
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

#include "moc_permissionsmanagertest.cpp"
