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
