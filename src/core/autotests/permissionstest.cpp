/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionstest.h"
using namespace Qt::Literals::StringLiterals;

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
#if 0
void PermissionsTest::shouldLoadPermissions_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("permissionsCount");

    QTest::addRow("permissions1") << u"permissions1"_s << 3;
}

void PermissionsTest::shouldLoadPermissions()
{
    QFETCH(QString, name);
    QFETCH(int, permissionsCount);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/permissions/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    Permissions r;
    r.p(obj);
    QCOMPARE(r.roles().count(), rolesCount);
}
#endif

#include "moc_permissionstest.cpp"
