/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ownuserpreferencestest.h"
#include "ownuser/ownuserpreferences.h"
#include "ruqola_autotest_helper.h"
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(OwnUserPreferencesTest)
OwnUserPreferencesTest::OwnUserPreferencesTest(QObject *parent)
    : QObject(parent)
{
}

void OwnUserPreferencesTest::shouldHaveDefaultValues()
{
    OwnUserPreferences p;
    QVERIFY(p.highlightWords().isEmpty());
    QVERIFY(p.emailNotificationMode().isEmpty());
    QVERIFY(p.desktopNotifications().isEmpty());
    QVERIFY(p.pushNotifications().isEmpty());
    QVERIFY(p.convertAsciiEmoji());
    QVERIFY(p.useEmojis());
    QVERIFY(!p.hideRoles());
    QVERIFY(p.displayAvatars());
    QCOMPARE(p.idleTimeLimit(), -1);
    QVERIFY(!p.showUnread());
    QVERIFY(!p.showRoomAvatar());
    QVERIFY(p.showFavorite());
    QCOMPARE(p.roomListSortOrder(), OwnUserPreferences::RoomListSortOrder::Unknown);
    QCOMPARE(p.roomListDisplay(), OwnUserPreferences::RoomListDisplay::Unknown);
}

void OwnUserPreferencesTest::shouldLoadOwnUserPreferencesInfo_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<OwnUserPreferences>("ownUserPreferences");
    {
        OwnUserPreferences preferences;
        preferences.setShowFavorite(false);
        preferences.setReceiveLoginDetectionEmail(true);
        QTest::addRow("empty") << QStringLiteral("empty") << preferences;
    }
    {
        OwnUserPreferences preferences;
        preferences.setEmailNotificationMode(QStringLiteral("mentions"));
        preferences.setDesktopNotifications(QStringLiteral("mentions"));
        preferences.setConvertAsciiEmoji(false);
        preferences.setUseEmojis(false);
        preferences.setIdleTimeLimit(300);
        preferences.setDisplayAvatars(true);
        preferences.setShowFavorite(true);
        preferences.setRoomListSortOrder(OwnUserPreferences::RoomListSortOrder::ByLastMessage);
        preferences.setRoomListDisplay(OwnUserPreferences::RoomListDisplay::Medium);
        preferences.setReceiveLoginDetectionEmail(true);
        preferences.setNewRoomNotification(QStringLiteral("door"));
        preferences.setNewMessageNotification(QStringLiteral("chime"));
        QTest::addRow("ownuserpreferences1") << QStringLiteral("ownuserpreferences1") << preferences;
    }
}

void OwnUserPreferencesTest::shouldLoadOwnUserPreferencesInfo()
{
    QFETCH(QString, fileName);
    QFETCH(OwnUserPreferences, ownUserPreferences);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/ownuserpreferences/"_L1 + fileName + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    OwnUserPreferences r;
    r.parsePreferences(obj);
    const bool result = (r == ownUserPreferences);
    if (!result) {
        qDebug() << "result " << r;
        qDebug() << "expected " << ownUserPreferences;
    }
    QVERIFY(result);
}

#include "moc_ownuserpreferencestest.cpp"
