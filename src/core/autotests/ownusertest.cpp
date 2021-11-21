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

#include "ownusertest.h"
#include "ownuser/ownuser.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
#include <QTest>
QTEST_GUILESS_MAIN(OwnUserTest)

OwnUserTest::OwnUserTest(QObject *parent)
    : QObject(parent)
{
}

void OwnUserTest::shouldHaveDefaultValues()
{
    OwnUser user;
    QVERIFY(user.userId().isEmpty());
    QVERIFY(user.userName().isEmpty());
    QVERIFY(user.status().isEmpty());
    QVERIFY(user.email().isEmpty());
    QVERIFY(user.statusText().isEmpty());
    QVERIFY(user.name().isEmpty());
    QVERIFY(user.avatarUrl().isEmpty());
    QVERIFY(user.nickName().isEmpty());
    QCOMPARE(user.utcOffset(), 0.0);
    QVERIFY(user.statusDefault().isEmpty());
    QVERIFY(user.roles().isEmpty());

    QVERIFY(!user.servicePassword().totp());
    QVERIFY(!user.servicePassword().email2faEnabled());
    QVERIFY(user.servicePassword().password().isEmpty());
}

void OwnUserTest::shouldLoadOwnUser_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<OwnUser>("ownuser");
    OwnUser result;
    result.setUserId(QStringLiteral("YbwG4T2uB3wZSZSKB"));
    result.setStatus(QStringLiteral("online"));
    result.setUserName(QStringLiteral("foo-bla"));
    result.setEmail(QStringLiteral("bla@kde.org"));
    result.setName(QStringLiteral("foo-name"));
    result.setAvatarUrl(QStringLiteral("https://open.rocket.chat/avatar/fooblabla"));
    result.setStatusDefault(QStringLiteral("online"));
    result.setNickName(QStringLiteral("tests"));
    result.setRoles({QStringLiteral("user")});
    result.setUtcOffset(2);
    ServicePassword pwd;
    pwd.setEmail2faEnabled(true);
    pwd.setTotp(false);
    pwd.setPassword(QStringLiteral("$2BNmO77i"));
    result.setServicePassword(pwd);

    OwnUserPreferences ownUser;
    ownUser.setDesktopNotifications(QStringLiteral("all"));
    ownUser.setMobileNotifications(QStringLiteral("all"));
    ownUser.setEmailNotificationMode(QStringLiteral("mentions"));
    ownUser.setIdleTimeLimit(300);
    result.setOwnUserPreferences(ownUser);

    QTest::addRow("example1") << QStringLiteral("example1") << result;
}

void OwnUserTest::shouldLoadOwnUser()
{
    QFETCH(QString, name);
    QFETCH(OwnUser, ownuser);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/ownuser/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    OwnUser r;
    r.parseOwnUserInfo(obj);
    const bool equalOwner = (r == ownuser);
    if (!equalOwner) {
        qDebug() << "ACTUAL " << r;
        qDebug() << "EXPECTED " << ownuser;
    }
    QVERIFY(equalOwner);
}
