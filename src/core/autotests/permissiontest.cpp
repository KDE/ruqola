/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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
