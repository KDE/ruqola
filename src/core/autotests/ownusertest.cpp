/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    ownUser.setEmailNotificationMode(QStringLiteral("mentions"));
    ownUser.setIdleTimeLimit(300);
    ownUser.setEnableAutoAway(true);
    ownUser.setMessageViewMode(0);
    ownUser.setDisplayAvatars(false);
    ownUser.setShowFavorite(true);
    ownUser.setRoomListSortOrder(OwnUserPreferences::RoomListSortOrder::ByLastMessage);
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
