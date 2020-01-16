/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

#include "rolestest.h"
#include "roles.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(RolesTest)

RolesTest::RolesTest(QObject *parent)
    : QObject(parent)
{
}

void RolesTest::shouldHaveDefaultValue()
{
    Roles r;
    QVERIFY(r.roles().isEmpty());
    QVERIFY(r.isEmpty());
}

void RolesTest::shouldFindRoles()
{
    Roles r;

    QVector<Role> v;

    Role r1;
    r1.setIsOwner(true);
    r1.setIsLeader(true);
    r1.setUserId(QStringLiteral("r1"));
    v.append(r1);

    Role r2;
    r2.setIsOwner(true);
    r2.setIsLeader(false);
    r2.setUserId(QStringLiteral("r2"));
    v.append(r2);

    r.setRoles(v);

    Role result = r.findRoleByUserId(QStringLiteral("r1"));
    QVERIFY(result.isValid());
    QVERIFY(result.isOwner());
    QVERIFY(result.isLeader());
    QVERIFY(!result.isModerator());

    result = r.findRoleByUserId(QStringLiteral("r7"));
    QVERIFY(!result.isValid());
}

void RolesTest::shouldLoadRoles_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("rolesCount");

    QTest::addRow("roles1") << QStringLiteral("roles1") << 3;
}

void RolesTest::shouldLoadRoles()
{
    QFETCH(QString, name);
    QFETCH(int, rolesCount);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/rolesforroom/") + name + QLatin1String(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject obj = doc.object();

    Roles r;
    r.parseRole(obj);
    QCOMPARE(r.roles().count(), rolesCount);
}
