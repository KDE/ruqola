/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roleinfotest.h"
#include "roles/roleinfo.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(RoleInfoTest)
RoleInfoTest::RoleInfoTest(QObject *parent)
    : QObject(parent)
{
}

void RoleInfoTest::shouldHaveDefaultValues()
{
    RoleInfo w;
    QVERIFY(w.name().isEmpty());
    QVERIFY(w.identifier().isEmpty());
    QVERIFY(w.scope().isEmpty());
    QVERIFY(w.description().isEmpty());
    QVERIFY(!w.roleProtected());
}

void RoleInfoTest::shouldLoadRoles_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<RoleInfo>("role");
    {
        RoleInfo r;
        r.setMandatory2fa(false);
        r.setRoleProtected(true);
        r.setName(QStringLiteral("app"));
        r.setScope(QStringLiteral("Users"));
        r.setIdentifier(QStringLiteral("app"));

        QTest::addRow("roleinfo1") << QStringLiteral("roleinfo1") << r;
    }
    {
        RoleInfo r;
        r.setMandatory2fa(false);
        r.setRoleProtected(true);
        r.setDescription(QStringLiteral("Livechat Manager"));
        r.setName(QStringLiteral("livechat-manager"));
        r.setScope(QStringLiteral("Users"));
        r.setIdentifier(QStringLiteral("livechat-manager"));

        QTest::addRow("roleinfo2") << QStringLiteral("roleinfo2") << r;
    }
}

void RoleInfoTest::shouldLoadRoles()
{
    QFETCH(QString, name);
    QFETCH(RoleInfo, role);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/rolesinfo/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    RoleInfo r;
    r.parseRoleInfo(obj);
    QCOMPARE(r, role);
}

#include "moc_roleinfotest.cpp"
