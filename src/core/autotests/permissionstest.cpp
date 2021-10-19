/*
   Copyright (c) 2019-2021 Laurent Montel <montel@kde.org>

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

#include "permissionstest.h"
#include "permissions/permissions.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
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
#if 0 // Add test for loading
void PermissionsTest::shouldLoadPermissions_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("rolesCount");

    QTest::addRow("roles1") << QStringLiteral("roles1") << 3;
}

void PermissionsTest::shouldLoadPermissions()
{
    QFETCH(QString, name);
    QFETCH(int, rolesCount);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/permissions/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    Roles r;
    r.parseRole(obj);
    QCOMPARE(r.roles().count(), rolesCount);
}
#endif
