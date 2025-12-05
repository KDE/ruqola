/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roleinfotest.h"
#include "roles/roleinfo.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
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
        r.setName(u"app"_s);
        r.setScope(u"Users"_s);
        r.setIdentifier(u"app"_s);

        QTest::addRow("roleinfo1") << u"roleinfo1"_s << r;
    }
    {
        RoleInfo r;
        r.setMandatory2fa(false);
        r.setRoleProtected(true);
        r.setDescription(u"Livechat Manager"_s);
        r.setName(u"livechat-manager"_s);
        r.setScope(u"Users"_s);
        r.setIdentifier(u"livechat-manager"_s);

        QTest::addRow("roleinfo2") << u"roleinfo2"_s << r;
    }
}

void RoleInfoTest::shouldLoadRoles()
{
    QFETCH(QString, name);
    QFETCH(RoleInfo, role);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/rolesinfo/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    RoleInfo r;
    r.parseRoleInfo(obj);
    QCOMPARE(r, role);
}

#include "moc_roleinfotest.cpp"
