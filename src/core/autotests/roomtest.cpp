/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomtest.h"
using namespace Qt::Literals::StringLiterals;

#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
#include "room.h"
#include "ruqola_autotest_helper.h"
#include <QCborValue>
#include <QJsonDocument>
#include <QSignalSpy>
#include <QTest>

QTEST_GUILESS_MAIN(RoomTest)

RoomTest::RoomTest(QObject *parent)
    : QObject(parent)
{
}

void RoomTest::shouldHaveDefaultValue()
{
    Room input(nullptr);
    QVERIFY(input.usersModelForRoom());

    QVERIFY(input.messageModel());

    QVERIFY(input.description().isEmpty());
    QVERIFY(input.announcement().isEmpty());
    QVERIFY(!input.readOnly());
    // Add more
    QCOMPARE(input.userMentions(), 0);
    QCOMPARE(input.groupMentions(), 0);
    QCOMPARE(input.unread(), 0);
    QCOMPARE(input.blocked(), false);
    QCOMPARE(input.blocker(), false);
    QVERIFY(input.roles().isEmpty());
    QVERIFY(!input.archived());
    QVERIFY(input.name().isEmpty());
    QVERIFY(input.ignoredUsers().isEmpty());
    QVERIFY(input.e2EKey().isEmpty());
    QVERIFY(input.e2eKeyId().isEmpty());
    QVERIFY(!input.encrypted());
    QVERIFY(!input.alert());
    QVERIFY(!input.readOnly());
    QVERIFY(!input.joinCodeRequired());
    QVERIFY(!input.wasInitialized());
    QVERIFY(input.rolesForRooms().isEmpty());
    QVERIFY(input.parentRid().isEmpty());
    QVERIFY(!input.broadcast());
    QVERIFY(input.fName().isEmpty());
    QVERIFY(input.autoTranslateLanguage().isEmpty());
    QVERIFY(!input.autoTranslate());
    QCOMPARE(input.lastSeenAt(), -1);
    QVERIFY(input.directChannelUserId().isEmpty());
    QVERIFY(input.displaySystemMessageTypes().isEmpty());
    QVERIFY(input.highlightsWord().isEmpty());
    QCOMPARE(input.lastMessageAt(), -1);
    QCOMPARE(input.numberMessages(), 0);
    QVERIFY(input.uids().isEmpty());

    const RetentionInfo info = input.retentionInfo();
    QVERIFY(!info.enabled());
    QVERIFY(!info.overrideGlobal());
    QVERIFY(!info.excludePinned());
    QVERIFY(!info.filesOnly());
    QCOMPARE(info.maxAge(), -1);

    const TeamInfo teamInfo = input.teamInfo();
    QVERIFY(teamInfo.teamId().isEmpty());
    QVERIFY(!teamInfo.mainTeam());
    QVERIFY(!teamInfo.autoJoin());
    QVERIFY(!teamInfo.isValid());
    QVERIFY(!teamInfo.hasTeamRoom());
    QCOMPARE(teamInfo.roomsCount(), 0);

    const NotificationOptions w = input.notificationOptions();
    QVERIFY(w.desktopNotifications().isEmpty());
    QVERIFY(w.mobilePushNotification().isEmpty());
    QVERIFY(w.emailNotifications().isEmpty());
    QVERIFY(w.unreadTrayIconAlert().isEmpty());
    QVERIFY(!w.disableNotifications());
    QVERIFY(!w.hideUnreadStatus());
    QVERIFY(!w.muteGroupMentions());
    QVERIFY(w.audioNotificationValue().isEmpty());

    QCOMPARE(input.lastOpenedAt(), -1);

    // 26/03/2024: size: 1040
    QCOMPARE(sizeof(Room), 792);
}

void RoomTest::shouldSerialized()
{
    {
        Room input(nullptr);
        input.setRoomId("foo"_ba);
        input.setChannelType(Room::roomTypeFromString(u"p"_s));
        input.setName(u"d"_s);
        input.setAnnouncement(u"AA"_s);
        input.setRoomCreatorUserName(u"pp"_s);
        input.setRoomCreatorUserId("sdfsdfs"_ba);
        input.setTopic(u"topic"_s);
        input.setMutedUsers(QStringList{u"mutedUsers"_s, u"muted2"_s});
        input.setJitsiTimeout(55);
        input.setReadOnly(true);
        input.setUnread(66);
        input.setSelected(true);
        input.setFavorite(true);
        input.setAlert(true);
        input.setOpen(true);
        input.setBlocker(true);
        input.setBlocked(true);
        input.setArchived(true);
        input.setDescription(u"dd"_s);
        input.setUserMentions(3);
        input.setRoles({u"foo"_s, u"bla"_s});
        input.setIgnoredUsers({u"gg"_s, u"gg2"_s});
        input.setJoinCodeRequired(true);
        input.setAutoTranslateLanguage(u"blss"_s);
        input.setAutoTranslate(true);
        input.setLastSeenAt(253);

        // RetentionInfo
        RetentionInfo info;
        info.setEnabled(true);
        info.setOverrideGlobal(true);
        info.setExcludePinned(true);
        info.setFilesOnly(true);
        info.setMaxAge(12);
        input.setRetentionInfo(info);

        const QByteArray ba = Room::serialize(&input);
        // qDebug() << QJsonObject(QJsonDocument::fromBinaryData(ba).object());
        // Room *output = Room::fromJSon(QJsonObject(QJsonDocument::fromBinaryData(ba).object()));
        auto output = Room::deserialize(QCborValue::fromCbor(ba).toMap().toJsonObject());
        // qDebug() << "after" << QJsonObject(QJsonDocument::fromBinaryData(Room::serialize(output)).object());
        // qDebug() << "input  " << input;
        // qDebug() << "output  " << *output;
        QVERIFY(input.isEqual(*output));
    }
    {
        Room input(nullptr);
        input.setRoomId("foo"_ba);
        input.setChannelType(Room::roomTypeFromString(u"p"_s));
        input.setName(u"p"_s);
        input.setAnnouncement(u"AA"_s);
        input.setRoomCreatorUserName(u"pp"_s);
        input.setRoomCreatorUserId("sdfsdfs"_ba);
        input.setTopic(u"topic"_s);
        input.setMutedUsers(QStringList{u"mutedUsers"_s, u"muted2"_s});
        input.setJitsiTimeout(55);
        input.setReadOnly(true);
        input.setUnread(66);
        input.setSelected(true);
        input.setFavorite(true);
        input.setAlert(true);
        input.setOpen(true);
        input.setBlocker(true);
        input.setBlocked(true);
        input.setArchived(true);
        input.setDescription(u"dd"_s);
        input.setUserMentions(3);
        input.setRoles({u"foo"_s, u"bla"_s});
        input.setIgnoredUsers({u"gg"_s, u"gg2"_s});
        input.setJoinCodeRequired(true);
        input.setAutoTranslateLanguage(u"blss"_s);
        input.setAutoTranslate(true);
        input.setLastSeenAt(253);

        // RetentionInfo
        RetentionInfo info;
        info.setEnabled(true);
        info.setOverrideGlobal(false);
        info.setExcludePinned(false);
        info.setFilesOnly(false);
        info.setMaxAge(12);
        input.setRetentionInfo(info);

        // TeamInfo
        TeamInfo teamInfo;
        teamInfo.setTeamId("dddd"_ba);
        teamInfo.setMainTeam(true);
        teamInfo.setAutoJoin(true);
        teamInfo.setRoomsCount(12);
        input.setTeamInfo(teamInfo);

        // Notification
        NotificationOptions w;
        NotificationOptions::NotificationValue w1;
        w1.value = "notification1"_ba;
        w.setDesktopNotifications(w1);
        NotificationOptions::NotificationValue w2;
        w2.value = "notification2"_ba;
        w.setMobilePushNotification(w2);
        NotificationOptions::NotificationValue w3;
        w3.value = "notification3"_ba;
        w.setEmailNotifications(w3);
        w.setUnreadTrayIconAlert(u"ssssf"_s);
        w.setDisableNotifications(false);
        w.setHideUnreadStatus(true);
        w.setMuteGroupMentions(true);
        w.setAudioNotificationValue("test1"_ba);
        input.setNotificationOptions(w);

        // setParentRid
        input.setParentRid("parentId1"_ba);

        // Avatar
        input.setAvatarETag("avatar1"_ba);

        // setE2EKey
        input.setE2EKey(u"e2ekey-1"_s);
        input.setE2eKeyId(u"e2ekey-id-1"_s);

        // setEncrypted
        input.setEncrypted(true);

        // setBroadcast
        input.setBroadcast(true);

        // setDisplaySystemMessageTypes
        input.setDisplaySystemMessageTypes({u"bla"_s, u"foo"_s});

        // setNumberMessages
        input.setNumberMessages(5);

        // setUpdatedAt
        input.setUpdatedAt(5555);

        // setHighlightsWord
        input.setHighlightsWord({u"highlight-bla"_s, u"highlight-foo"_s});

        // uids
        input.setUids({u"uids-bla"_s, u"uids-foo"_s});

        const QByteArray ba = Room::serialize(&input);
        // qDebug() << QJsonObject(QJsonDocument::fromBinaryData(ba).object());
        // Room *output = Room::fromJSon(QJsonObject(QJsonDocument::fromBinaryData(ba).object()));
        auto output = Room::deserialize(QCborValue::fromCbor(ba).toMap().toJsonObject());
        // qDebug() << "after" << QJsonObject(QJsonDocument::fromBinaryData(Room::serialize(output)).object());
        // qDebug() << "input  " << input;
        // qDebug() << "output  " << *output;
        QVERIFY(input.isEqual(*output));
    }
}

void RoomTest::shouldEmitSignals()
{
    Room input(nullptr);
    QSignalSpy spyNameChanged(&input, &Room::nameChanged);
    QSignalSpy spyannouncementChanged(&input, &Room::announcementChanged);
    QSignalSpy spytopicChanged(&input, &Room::topicChanged);
    QSignalSpy spyfavoriteChanged(&input, &Room::favoriteChanged);
    QSignalSpy spyalertChanged(&input, &Room::alertChanged);
    QSignalSpy spyreadOnlyChanged(&input, &Room::readOnlyChanged);
    QSignalSpy spyunreadChanged(&input, &Room::unreadChanged);
    QSignalSpy spyblockerChanged(&input, &Room::blockerChanged);
    QSignalSpy spyarchivedChanged(&input, &Room::archivedChanged);
    QSignalSpy spydescriptionChanged(&input, &Room::descriptionChanged);
    QSignalSpy spyblockedChanged(&input, &Room::blockedChanged);
    QSignalSpy spyrolesChanged(&input, &Room::rolesChanged);
    QSignalSpy spyignoredUsersChanged(&input, &Room::ignoredUsersChanged);
    QSignalSpy spymutedUsersChanged(&input, &Room::mutedUsersChanged);
    QSignalSpy spyencryptedChanged(&input, &Room::encryptedChanged);
    QSignalSpy spyjoinCodeRequiredChanged(&input, &Room::joinCodeRequiredChanged);
    QSignalSpy spychannelTypeChanged(&input, &Room::channelTypeChanged);
    QSignalSpy spyparentRidChanged(&input, &Room::parentRidChanged);
    QSignalSpy spyautoTranslateLanguageChanged(&input, &Room::autoTranslateLanguageChanged);
    QSignalSpy spyautoTranslateChanged(&input, &Room::autoTranslateChanged);
    QSignalSpy spydirectChannelUserIdChanged(&input, &Room::directChannelUserIdChanged);
    QSignalSpy spylastMessageAtChanged(&input, &Room::lastMessageAtChanged);

    input.setRoomId("foo"_ba);
    input.setChannelType(Room::roomTypeFromString(u"p"_s));
    input.setName(u"d"_s);
    input.setAnnouncement(u"AA"_s);
    input.setRoomCreatorUserName(u"pp"_s);
    input.setRoomCreatorUserId("sdfsdfs"_ba);
    input.setTopic(u"topic"_s);
    input.setMutedUsers(QStringList{u"mutedUsers"_s, u"muted2"_s});
    input.setJitsiTimeout(55);
    input.setReadOnly(true);
    input.setUnread(66);
    input.setSelected(true);
    input.setFavorite(true);
    input.setAlert(true);
    input.setOpen(true);
    input.setBlocker(true);
    input.setBlocked(true);
    input.setArchived(true);
    input.setEncrypted(true);
    input.setJoinCodeRequired(true);
    input.setDescription(u"ddd"_s);
    input.setRoles({u"bla"_s, u"blu"_s});
    input.setIgnoredUsers({u"bla"_s, u"blu3"_s});
    input.setParentRid("bla"_ba);
    input.setAutoTranslateLanguage(u"bli"_s);
    input.setAutoTranslate(true);
    input.setDirectChannelUserId("naninani"_ba);
    input.setLastMessageAt(QDateTime::currentMSecsSinceEpoch());

    QCOMPARE(spyNameChanged.count(), 1);
    QCOMPARE(spyannouncementChanged.count(), 1);
    QCOMPARE(spytopicChanged.count(), 1);
    QCOMPARE(spyfavoriteChanged.count(), 1);
    QCOMPARE(spyalertChanged.count(), 1);
    QCOMPARE(spyreadOnlyChanged.count(), 1);
    QCOMPARE(spyunreadChanged.count(), 1);
    QCOMPARE(spyblockerChanged.count(), 1);
    QCOMPARE(spyarchivedChanged.count(), 1);
    QCOMPARE(spyblockedChanged.count(), 1);
    QCOMPARE(spydescriptionChanged.count(), 1);
    QCOMPARE(spyrolesChanged.count(), 1);
    QCOMPARE(spyignoredUsersChanged.count(), 1);
    QCOMPARE(spymutedUsersChanged.count(), 1);
    QCOMPARE(spyencryptedChanged.count(), 1);
    QCOMPARE(spyjoinCodeRequiredChanged.count(), 1);
    QCOMPARE(spychannelTypeChanged.count(), 1);
    QCOMPARE(spyparentRidChanged.count(), 1);
    QCOMPARE(spyautoTranslateLanguageChanged.count(), 1);
    QCOMPARE(spyautoTranslateChanged.count(), 1);
    QCOMPARE(spydirectChannelUserIdChanged.count(), 1);
    QCOMPARE(spylastMessageAtChanged.count(), 1);
}

void RoomTest::shoudUserIsMuted()
{
    {
        Room input(nullptr);
        input.setMutedUsers(QStringList());
        QVERIFY(!input.userIsMuted("foo"_L1));

        QStringList lst;
        lst << u"bla"_s << u"foo"_s;
        input.setMutedUsers(lst);
        QVERIFY(input.userIsMuted("foo"_L1));
    }
}

void RoomTest::shouldParseRoom_data()
{
    QTest::addColumn<QString>("fileName");
    // Missing _updatedAt/ts/_id/groupMentions/ls/roles (implement roles ! )
    QTest::newRow("notification-room") << u"notification-room"_s;

    QTest::newRow("unread-usermentions-room") << u"unread-usermentions-room"_s;
    QTest::newRow("muted-users") << u"muted-users"_s;
    QTest::newRow("userignored-room") << u"userignored-room"_s;
    QTest::newRow("autotranslatelanguage") << u"autotranslatelanguage"_s;
    QTest::newRow("direct-room") << u"direct-room"_s;
    QTest::newRow("retention-room") << u"retention-room"_s;
    QTest::newRow("team-room") << u"team-room"_s;
}

void RoomTest::shouldParseRoom()
{
    QFETCH(QString, fileName);

    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/room/"_L1 + fileName + ".json"_L1;
    const QJsonObject fields = AutoTestHelper::loadJsonObject(originalJsonFile);

    Room r;
    r.parseSubscriptionRoom(fields);
    // qDebug() << " fields"<<fields;

    const QByteArray ba = Room::serialize(&r, false);
    // qDebug() << " ba " << ba;
    const QJsonDocument docSerialized = QJsonDocument::fromJson(ba);

    const QByteArray jsonIndented = docSerialized.toJson(QJsonDocument::Indented);
    AutoTestHelper::compareFile(u"/room/"_s, jsonIndented, fileName);

    auto m = Room::deserialize(docSerialized.object());
    QCOMPARE(r, *m);
}

void RoomTest::shouldParseRoomAndUpdate_data()
{
    QTest::addColumn<QString>("fileNameinit");
    QTest::addColumn<QStringList>("fileNameupdate");
    // Missing _updatedAt/ts/_id/groupMentions/ls/roles (implement roles ! )
    QTest::newRow("notification-roomupdate") << u"notification-room"_s << (QStringList() << u"notification-roomupdate1"_s);
    QTest::newRow("room-update") << u"room-update"_s << (QStringList() << u"room-update1"_s);
    QTest::newRow("room-without-owner") << u"room-without-owner"_s << (QStringList() << u"room-without-owner1"_s);
    QTest::newRow("room-mute-unmute") << u"room-mute-unmute"_s << (QStringList() << u"muted-users"_s << u"unmuted-users"_s);
    QTest::newRow("userignored-room") << u"userignored-room"_s << (QStringList() << u"userignored-room-update"_s);
    QTest::newRow("room-requiredjoincode-owner") << u"room-requiredjoincode-owner"_s << (QStringList() << u"room-requiredjoincode-update"_s);
    QTest::newRow("autotranslatelanguage") << u"autotranslatelanguage"_s << (QStringList() << u"autotranslatelanguage-update"_s);
    QTest::newRow("direct-room") << u"direct-room"_s << (QStringList() << u"direct-room-update"_s);
    QTest::newRow("room-retention") << u"room-retention"_s << (QStringList() << u"room-retention-update"_s);
    QTest::newRow("room-team") << u"room-team"_s << (QStringList() << u"room-team"_s);
}

void RoomTest::shouldParseRoomAndUpdate()
{
    QFETCH(QString, fileNameinit);
    QFETCH(QStringList, fileNameupdate);

    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/room-updated/"_L1 + fileNameinit + ".json"_L1;
    QFile f(originalJsonFile);
    bool opened = f.open(QIODevice::ReadOnly);
    if (!opened) {
        qWarning() << " impossible to open " << originalJsonFile;
    }
    QVERIFY(opened);
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object();

    RocketChatAccount account;
    account.settings()->setUserId("uKK39zoewTkdacidH"_ba);
    Room r(&account);
    r.parseSubscriptionRoom(fields);

    for (const QString &updateFile : fileNameupdate) {
        const QString originalUpdateJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/room-updated/"_L1 + updateFile + ".json"_L1;
        const QJsonObject fields = AutoTestHelper::loadJsonObject(originalUpdateJsonFile);

        r.parseUpdateRoom(fields);
    }

    // qDebug() << " fields"<<fields;

    const QByteArray ba = Room::serialize(&r, false);
    // qDebug() << " ba " << ba;
    const QJsonDocument docSerialized = QJsonDocument::fromJson(ba);

    const QByteArray jsonIndented = docSerialized.toJson(QJsonDocument::Indented);
    AutoTestHelper::compareFile(u"/room-updated/"_s, jsonIndented, fileNameinit);

    auto m = Room::deserialize(docSerialized.object());
    QCOMPARE(r, *m);
}

void RoomTest::shouldParseRoomAndUpdateSubscription_data()
{
    QTest::addColumn<QString>("fileNameinit");
    QTest::addColumn<QStringList>("UpdateRoomfileNames");
    QTest::addColumn<QStringList>("UpdateSubscriptionFileNames");
    // Missing _updatedAt/ts/_id/groupMentions/ls/roles (implement roles ! )
    QTest::newRow("notification-roomupdate") << u"notification-room"_s << (QStringList() << u"notification-roomupdate1"_s)
                                             << (QStringList() << u"notification-roomsubscription1"_s);

    QTest::newRow("room-blocked") << u"room-blocked"_s << (QStringList() << u"room-blockedupdate1"_s) << QStringList();

    QTest::newRow("room-encryption") << u"room-encryption"_s << (QStringList() << u"room-encryptionupdate1"_s) << QStringList();

    QTest::newRow("room-broadcasted") << u"room-broadcasted"_s << (QStringList() << u"room-broadcastedupdate1"_s) << QStringList();
}

void RoomTest::shouldParseRoomAndUpdateSubscription()
{
    QFETCH(QString, fileNameinit);
    QFETCH(QStringList, UpdateRoomfileNames);
    QFETCH(QStringList, UpdateSubscriptionFileNames);

    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/room-updated/"_L1 + fileNameinit + ".json"_L1;
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object();

    Room r;
    r.parseSubscriptionRoom(fields);

    for (const QString &updateFile : UpdateRoomfileNames) {
        const QString originalUpdateJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/room-update-subscription/"_L1 + updateFile + ".json"_L1;
        QFile f(originalUpdateJsonFile);
        QVERIFY(f.open(QIODevice::ReadOnly));
        const QByteArray udpateSubscriptionContent = f.readAll();
        f.close();
        const QJsonDocument docSubscription = QJsonDocument::fromJson(udpateSubscriptionContent);
        const QJsonObject fields = docSubscription.object();

        r.parseUpdateRoom(fields);
    }

    for (const QString &updateFile : UpdateSubscriptionFileNames) {
        const QString originalUpdateJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/room-update-subscription/"_L1 + updateFile + ".json"_L1;
        const QJsonObject fields = AutoTestHelper::loadJsonObject(originalUpdateJsonFile);

        r.updateSubscriptionRoom(fields);
    }

    // qDebug() << " fields"<<fields;

    const QByteArray ba = Room::serialize(&r, false);
    // qDebug() << " ba " << ba;
    const QJsonDocument docSerialized = QJsonDocument::fromJson(ba);

    const QByteArray jsonIndented = docSerialized.toJson(QJsonDocument::Indented);
    AutoTestHelper::compareFile(u"/room-update-subscription/"_s, jsonIndented, fileNameinit);

    auto m = Room::deserialize(docSerialized.object());
    QCOMPARE(r, *m);
}

// TODO add more autotests signal and co.

#include "moc_roomtest.cpp"
