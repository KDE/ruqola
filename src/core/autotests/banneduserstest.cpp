/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "banneduserstest.h"
#include "bannedusers/bannedusers.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
#include <QTest>
QTEST_GUILESS_MAIN(BannedUsersTest)
using namespace Qt::Literals::StringLiterals;
BannedUsersTest::BannedUsersTest(QObject *parent)
    : QObject{parent}
{
}

void BannedUsersTest::shouldHaveDefaultValues()
{
    const BannedUsers w;
    QVERIFY(w.isEmpty());
    QVERIFY(w.bannedUsers().isEmpty());
    QCOMPARE(w.offset(), 0);
    QCOMPARE(w.total(), 0);
    QCOMPARE(w.bannedUsersCount(), 0);
}

void BannedUsersTest::shouldLoadBannedUsers_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("bannedUsersCount");
    QTest::addColumn<int>("total");
    QTest::addColumn<QList<BannedUser>>("userlist");

    {
        QList<BannedUser> lst;
        QTest::addRow("bannedusers1") << u"bannedusers1"_s << 0 << 0 << lst;
    }
    {
        QList<BannedUser> lst;
        {
            BannedUser w;
            w.setUserName(u"foo1"_s);
            w.setName(u"name foo1"_s);
            w.setIdentifier(u"6WosXzDcZYJaoyan"_s);
            lst.append(w);
        }
        {
            BannedUser w;
            w.setUserName(u"kde1"_s);
            w.setName(u"name kde1"_s);
            w.setIdentifier(u"JZqJ5yAbgcgKDL4"_s);
            lst.append(w);
        }
        {
            BannedUser w;
            w.setUserName(u"plo"_s);
            w.setName(u"plobame"_s);
            w.setIdentifier(u"hGv7ntFhDxB7ePf"_s);
            lst.append(w);
        }
        QTest::addRow("bannedusers2") << u"bannedusers2"_s << 3 << 3 << lst;
    }
}

void BannedUsersTest::shouldLoadBannedUsers()
{
    QFETCH(QString, name);
    QFETCH(int, bannedUsersCount);
    QFETCH(int, total);
    QFETCH(QList<BannedUser>, userlist);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/bannedusers/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    BannedUsers r;
    r.parseBannedUsers(obj);
    QCOMPARE(r.bannedUsersCount(), bannedUsersCount);
    QCOMPARE(r.total(), total);
    QCOMPARE(r.bannedUsers(), userlist);
}

#include "moc_banneduserstest.cpp"
