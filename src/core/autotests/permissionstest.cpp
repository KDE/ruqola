/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionstest.h"
#include "permissions/permissions.h"
#include "ruqola_autotest_helper.h"

QTEST_GUILESS_MAIN(PermissionsTest)

PermissionsTest::PermissionsTest(QObject *parent)
    : QObject(parent)
{
}

void PermissionsTest::shouldHaveDefaultValue()
{
    Permissions r;
    QVERIFY(r.permissions().isEmpty());
    QVERIFY(r.isEmpty());
}
#if 0
void PermissionsTest::shouldLoadPermissions_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("permissionsCount");

    QTest::addRow("permissions1") << QStringLiteral("permissions1") << 3;
}

void PermissionsTest::shouldLoadPermissions()
{
    QFETCH(QString, name);
    QFETCH(int, permissionsCount);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/permissions/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    Permissions r;
    r.p(obj);
    QCOMPARE(r.roles().count(), rolesCount);
}
#endif

#include "moc_permissionstest.cpp"
