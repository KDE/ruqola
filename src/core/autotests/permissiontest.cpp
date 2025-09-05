/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissiontest.h"
#include "permissions/permission.h"
#include "ruqola_autotest_helper.h"
using namespace Qt::Literals::StringLiterals;
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
        p.setIdentifier(u"pin-message"_s);
        p.setRoles({u"owner"_s, u"moderator"_s, u"admin"_s, u"global-moderator"_s});
        p.setUpdatedAt(1634615770401);
        QTest::addRow("permission1") << u"permission1"_s << p;
    }
    {
        Permission p;
        p.setIdentifier(u"create-c"_s);
        p.setRoles({u"admin"_s, u"user"_s, u"bot"_s, u"app"_s});
        p.setUpdatedAt(1634615770401);
        QTest::addRow("permission2") << u"permission2"_s << p;
    }
}

void PermissionTest::shouldLoadPermission()
{
    QFETCH(QString, name);
    QFETCH(Permission, permission);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/permission/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    Permission r;
    r.parsePermission(obj);
    // qDebug() << " r " << r << " permission " << permission;
    QCOMPARE(r, permission);
}

#include "moc_permissiontest.cpp"
