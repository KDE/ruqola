/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissiontest.h"
#include "permissions/permission.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(PermissionTest)
PermissionTest::PermissionTest(QObject *parent)
    : QObject(parent)
{
}

void PermissionTest::shouldHaveDefaultValues()
{
    Permission p;
    QVERIFY(p.roles().isEmpty());
    QVERIFY(p.rolesStr().isEmpty());
    QCOMPARE(p.updatedAt(), -1);
    QVERIFY(p.identifier().isEmpty());
}

void PermissionTest::shouldLoadPermission_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<Permission>("permission");

    {
        Permission p;
        p.setIdentifier(QStringLiteral("pin-message"));
        p.setRoles({QStringLiteral("owner"), QStringLiteral("moderator"), QStringLiteral("admin"), QStringLiteral("global-moderator")});
        p.setUpdatedAt(1634615770401);
        QTest::addRow("permission1") << QStringLiteral("permission1") << p;
    }
    {
        Permission p;
        p.setIdentifier(QStringLiteral("create-c"));
        p.setRoles({QStringLiteral("admin"), QStringLiteral("user"), QStringLiteral("bot"), QStringLiteral("app")});
        p.setUpdatedAt(1634615770401);
        QTest::addRow("permission2") << QStringLiteral("permission2") << p;
    }
}

void PermissionTest::shouldLoadPermission()
{
    QFETCH(QString, name);
    QFETCH(Permission, permission);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/permission/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    Permission r;
    r.parsePermission(obj);
    // qDebug() << " r " << r << " permission " << permission;
    QCOMPARE(r, permission);
}

#include "moc_permissiontest.cpp"
