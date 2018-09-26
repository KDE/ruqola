/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#include "roomtest.h"
#include "room.h"
#include "ruqola_autotest_helper.h"
#include "model/usersforroommodel.h"
#include "model/usersforroomfilterproxymodel.h"
#include "model/filesforroomfilterproxymodel.h"
#include "model/filesforroommodel.h"
#include <QJsonDocument>
#include <QTest>
#include <QSignalSpy>

QTEST_GUILESS_MAIN(RoomTest)

RoomTest::RoomTest(QObject *parent)
    : QObject(parent)
{
}

void RoomTest::shouldHaveDefaultValue()
{
    Room input(nullptr);
    QVERIFY(input.usersModelForRoom());
    QVERIFY(input.usersModelForRoomProxyModel());

    QVERIFY(input.filesModelForRoom());
    QVERIFY(input.filesForRoomFilterProxyModel());

    QCOMPARE(input.filesForRoomFilterProxyModel()->sourceModel(), input.filesModelForRoom());
    QCOMPARE(input.usersModelForRoomProxyModel()->sourceModel(), input.usersModelForRoom());

    QVERIFY(input.messageModel());
    QVERIFY(input.inputMessage().isEmpty());

    QVERIFY(input.description().isEmpty());
    QVERIFY(input.announcement().isEmpty());
    QVERIFY(!input.readOnly());
    //Add more
    QCOMPARE(input.userMentions(), 0);
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
}

//TODO add notification, userMentions too
void RoomTest::shouldSerialized()
{
    Room input(nullptr);
    input.setRoomId(QStringLiteral("foo"));
    input.setChannelType(QStringLiteral("p"));
    input.setName(QStringLiteral("d"));
    input.setAnnouncement(QStringLiteral("AA"));
    input.setRoomCreatorUserName(QStringLiteral("pp"));
    input.setRoomCreatorUserId(QStringLiteral("sdfsdfs"));
    input.setTopic(QStringLiteral("topic"));
    input.setMutedUsers(QStringList{QStringLiteral("mutedUsers"), QStringLiteral("muted2")});
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
    input.setDescription(QStringLiteral("dd"));
    input.setUserMentions(3);
    input.setRoles({QStringLiteral("foo"), QStringLiteral("bla")});
    input.setIgnoredUsers({QStringLiteral("gg"), QStringLiteral("gg2")});
    const QByteArray ba = Room::serialize(&input);
    //qDebug() << QJsonObject(QJsonDocument::fromBinaryData(ba).object());
    Room *output = Room::fromJSon(QJsonObject(QJsonDocument::fromBinaryData(ba).object()));
    //qDebug() << "after" << QJsonObject(QJsonDocument::fromBinaryData(Room::serialize(output)).object());
    QVERIFY(input.isEqual(*output));
    delete output;
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
    input.setRoomId(QStringLiteral("foo"));
    input.setChannelType(QStringLiteral("p"));
    input.setName(QStringLiteral("d"));
    input.setAnnouncement(QStringLiteral("AA"));
    input.setRoomCreatorUserName(QStringLiteral("pp"));
    input.setRoomCreatorUserId(QStringLiteral("sdfsdfs"));
    input.setTopic(QStringLiteral("topic"));
    input.setMutedUsers(QStringList{QStringLiteral("mutedUsers"), QStringLiteral("muted2")});
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
    input.setDescription(QStringLiteral("ddd"));
    input.setRoles({QStringLiteral("bla"), QStringLiteral("blu")});
    input.setIgnoredUsers({QStringLiteral("bla"), QStringLiteral("blu3")});

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
}

void RoomTest::shouldChangeInputMessage()
{
    Room input(nullptr);
    QString inputMsg = QStringLiteral("Foo");
    input.setInputMessage(inputMsg);
    QCOMPARE(input.inputMessage(), inputMsg);

    inputMsg = QString();
    input.setInputMessage(inputMsg);
    QCOMPARE(input.inputMessage(), inputMsg);

    inputMsg = QStringLiteral("foo");
    input.setInputMessage(inputMsg);
    QCOMPARE(input.inputMessage(), inputMsg);
}

void RoomTest::shouldParseRoom_data()
{
    QTest::addColumn<QString>("fileName");
    //Missing _updatedAt/ts/_id/groupMentions/ls/roles (implement roles ! )
    QTest::newRow("notification-room") << QStringLiteral("notification-room");

    QTest::newRow("unread-usermentions-room") << QStringLiteral("unread-usermentions-room");
    QTest::newRow("muted-users") << QStringLiteral("muted-users");
    QTest::newRow("userignored-room") << QStringLiteral("userignored-room");
}

void RoomTest::shouldParseRoom()
{
    QFETCH(QString, fileName);

    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/room/") + fileName + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object();

    Room r;
    r.parseSubscriptionRoom(fields);
    //qDebug() << " fields"<<fields;

    const QByteArray ba = Room::serialize(&r, false);
    //qDebug() << " ba " << ba;
    const QJsonDocument docSerialized = QJsonDocument::fromJson(ba);

    const QByteArray jsonIndented = docSerialized.toJson(QJsonDocument::Indented);
    compareFile(QStringLiteral("/room/"), jsonIndented, fileName);

    Room *m = Room::fromJSon(docSerialized.object());
    QCOMPARE(r, *m);
    delete m;
}

void RoomTest::shouldParseRoomAndUpdate_data()
{
    QTest::addColumn<QString>("fileNameinit");
    QTest::addColumn<QStringList>("fileNameupdate");
    //Missing _updatedAt/ts/_id/groupMentions/ls/roles (implement roles ! )
    QTest::newRow("notification-roomupdate") << QStringLiteral("notification-room") << (QStringList() <<QStringLiteral("notification-roomupdate1"));
    QTest::newRow("room-update") << QStringLiteral("room-update") << (QStringList() <<QStringLiteral("room-update1"));
    QTest::newRow("room-without-owner") << QStringLiteral("room-without-owner") << (QStringList() <<QStringLiteral("room-without-owner1"));
    QTest::newRow("room-mute-unmute") << QStringLiteral("room-mute-unmute") << (QStringList() <<QStringLiteral("muted-users") << QStringLiteral("unmuted-users"));
    QTest::newRow("userignored-room") << QStringLiteral("userignored-room") << (QStringList() <<QStringLiteral("userignored-room-update"));
}

void RoomTest::shouldParseRoomAndUpdate()
{
    QFETCH(QString, fileNameinit);
    QFETCH(QStringList, fileNameupdate);

    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/room-updated/") + fileNameinit + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object();

    Room r;
    r.parseSubscriptionRoom(fields);

    for (const QString &updateFile : fileNameupdate) {
        const QString originalUpdateJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/room-updated/") + updateFile + QStringLiteral(".json");
        QFile f(originalUpdateJsonFile);
        QVERIFY(f.open(QIODevice::ReadOnly));
        const QByteArray content = f.readAll();
        f.close();
        const QJsonDocument doc = QJsonDocument::fromJson(content);
        const QJsonObject fields = doc.object();

        r.parseUpdateRoom(fields);
    }

    //qDebug() << " fields"<<fields;

    const QByteArray ba = Room::serialize(&r, false);
    //qDebug() << " ba " << ba;
    const QJsonDocument docSerialized = QJsonDocument::fromJson(ba);

    const QByteArray jsonIndented = docSerialized.toJson(QJsonDocument::Indented);
    compareFile(QStringLiteral("/room-updated/"), jsonIndented, fileNameinit);

    Room *m = Room::fromJSon(docSerialized.object());
    QCOMPARE(r, *m);
    delete m;
}

void RoomTest::shouldParseRoomAndUpdateSubscription_data()
{
    QTest::addColumn<QString>("fileNameinit");
    QTest::addColumn<QStringList>("UpdateRoomfileNames");
    QTest::addColumn<QStringList>("UpdateSubscriptionFileNames");
    //Missing _updatedAt/ts/_id/groupMentions/ls/roles (implement roles ! )
    QTest::newRow("notification-roomupdate")
        << QStringLiteral("notification-room")
        << (QStringList() << QStringLiteral("notification-roomupdate1"))
        << (QStringList() << QStringLiteral("notification-roomsubscription1"));

    QTest::newRow("room-blocked")
        << QStringLiteral("room-blocked")
        << (QStringList() << QStringLiteral("room-blockedupdate1"))
        << QStringList();

    QTest::newRow("room-encryption")
        << QStringLiteral("room-encryption")
        << (QStringList() << QStringLiteral("room-encryptionupdate1"))
        << QStringList();

}

void RoomTest::shouldParseRoomAndUpdateSubscription()
{
    QFETCH(QString, fileNameinit);
    QFETCH(QStringList, UpdateRoomfileNames);
    QFETCH(QStringList, UpdateSubscriptionFileNames);

    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/room-updated/") + fileNameinit + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object();

    Room r;
    r.parseSubscriptionRoom(fields);

    for (const QString &updateFile : UpdateRoomfileNames) {
        const QString originalUpdateJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/room-update-subscription/") + updateFile + QStringLiteral(".json");
        QFile f(originalUpdateJsonFile);
        QVERIFY(f.open(QIODevice::ReadOnly));
        const QByteArray content = f.readAll();
        f.close();
        const QJsonDocument doc = QJsonDocument::fromJson(content);
        const QJsonObject fields = doc.object();

        r.parseUpdateRoom(fields);
    }

    for (const QString &updateFile : UpdateSubscriptionFileNames) {
        const QString originalUpdateJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/room-update-subscription/") + updateFile + QStringLiteral(".json");
        QFile f(originalUpdateJsonFile);
        QVERIFY(f.open(QIODevice::ReadOnly));
        const QByteArray content = f.readAll();
        f.close();
        const QJsonDocument doc = QJsonDocument::fromJson(content);
        const QJsonObject fields = doc.object();

        r.updateSubscriptionRoom(fields);
    }

    //qDebug() << " fields"<<fields;

    const QByteArray ba = Room::serialize(&r, false);
    //qDebug() << " ba " << ba;
    const QJsonDocument docSerialized = QJsonDocument::fromJson(ba);

    const QByteArray jsonIndented = docSerialized.toJson(QJsonDocument::Indented);
    compareFile(QStringLiteral("/room-update-subscription/"), jsonIndented, fileNameinit);

    Room *m = Room::fromJSon(docSerialized.object());
    QCOMPARE(r, *m);
    delete m;
}

//TODO add more autotests signal and co.
