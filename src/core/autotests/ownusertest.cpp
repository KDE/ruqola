/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ownusertest.h"
#include "ownuser/ownuser.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
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
    result.setUserId("YbwG4T2uB3wZSZSKB"_ba);
    result.setStatus(u"online"_s);
    result.setUserName(u"foo-bla"_s);
    result.setEmail(u"bla@kde.org"_s);
    result.setName(u"foo-name"_s);
    result.setAvatarUrl(u"https://open.rocket.chat/avatar/fooblabla"_s);
    result.setStatusDefault(u"online"_s);
    result.setNickName(u"tests"_s);
    result.setRoles({u"user"_s});
    result.setUtcOffset(2);
    ServicePassword pwd;
    pwd.setEmail2faEnabled(true);
    pwd.setTotp(false);
    pwd.setPassword(u"$2BNmO77i"_s);
    result.setServicePassword(pwd);

    OwnUserPreferences ownUser;
    ownUser.setDesktopNotifications(u"all"_s);
    ownUser.setEmailNotificationMode(u"mentions"_s);
    ownUser.setIdleTimeLimit(300);
    ownUser.setEnableAutoAway(true);
    ownUser.setDisplayAvatars(false);
    ownUser.setShowFavorite(true);
    ownUser.setRoomListSortOrder(OwnUserPreferences::RoomListSortOrder::ByLastMessage);
    ownUser.setRoomListDisplay(OwnUserPreferences::RoomListDisplay::Medium);
    ownUser.setReceiveLoginDetectionEmail(true);
    ownUser.setNewRoomNotification("door"_ba);
    ownUser.setNewMessageNotification("chime"_ba);
    ownUser.setNotificationsSoundVolume(100);
    ownUser.setMuteFocusedConversations(true);

    result.setOwnUserPreferences(ownUser);

    QTest::addRow("example1") << u"example1"_s << result;
}

void OwnUserTest::shouldLoadOwnUser()
{
    QFETCH(QString, name);
    QFETCH(OwnUser, ownuser);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/ownuser/"_L1 + name + ".json"_L1;
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

#include "moc_ownusertest.cpp"
