/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roommodeltest.h"
#include "model/roommodel.h"
#include "room.h"
#include "ruqola_autotest_helper.h"
#include "test_model_helpers.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>
#include <qglobal.h>
#include <rocketchataccount.h>

QTEST_GUILESS_MAIN(RoomModelTest)

// TEST signal/slot

RoomModelTest::RoomModelTest(QObject *parent)
    : QObject(parent)
{
}

void RoomModelTest::shouldHaveDefaultValues()
{
    RoomModel sampleModel;
    QCOMPARE(sampleModel.rowCount(), 0);
}

void RoomModelTest::shouldReturnRowCount()
{
    RoomModel sampleModel;

    QSignalSpy rowInsertedSpy(&sampleModel, &RoomModel::rowsInserted);
    QSignalSpy rowABTInserted(&sampleModel, &RoomModel::rowsAboutToBeInserted);
    QSignalSpy rowRemovedSpy(&sampleModel, &RoomModel::rowsRemoved);
    QSignalSpy rowABTRemoved(&sampleModel, &RoomModel::rowsAboutToBeRemoved);

    QCOMPARE(sampleModel.rowCount(), 0);

    sampleModel.addRoom(QStringLiteral("myRoomID1"), QStringLiteral("myRoom1"));
    QCOMPARE(sampleModel.rowCount(), 1);

    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,0"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,0"));

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    sampleModel.addRoom(QStringLiteral("myRoomID1"), QStringLiteral("bla bla"));
    QCOMPARE(sampleModel.rowCount(), 1);

    // Fix it. We remove + re-add after that ! it's not optimal
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    sampleModel.addRoom(QStringLiteral("myRoomID2"), QStringLiteral("myRoom2"), true);
    QCOMPARE(sampleModel.rowCount(), 2);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("1,1"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("1,1"));
}

void RoomModelTest::shouldFindRoom()
{
    RoomModel sampleModel;
    Room *room = nullptr;

    sampleModel.addRoom(QStringLiteral("RA15"), QStringLiteral("master"));
    room = sampleModel.findRoom(QStringLiteral("RA151100ECE"));
    QVERIFY(!room);

    sampleModel.addRoom(QStringLiteral("RA151100ECE"), QStringLiteral("myRoom"));
    room = sampleModel.findRoom(QStringLiteral("RA151100ECE"));
    QVERIFY(room);
    QCOMPARE(QStringLiteral("myRoom"), room->name());
}

void RoomModelTest::shouldAddRoom()
{
    RoomModel sampleModel;
    Room *room = nullptr;

    QCOMPARE(sampleModel.rowCount(), 0);
    sampleModel.addRoom(QStringLiteral("RA151100ECE"), QStringLiteral("myRoom"));
    QCOMPARE(sampleModel.rowCount(), 1);

    room = sampleModel.findRoom(QStringLiteral("RA151100ECE"));
    QVERIFY(room);
    QCOMPARE(QStringLiteral("myRoom"), room->name());
}

void RoomModelTest::shouldUpdateRoom()
{
    // FIXME!!
    /*
    RoomModel sampleModel;
    RoomWrapper *sampleWrapper = nullptr;

    sampleModel.addRoom(QStringLiteral("RA151100ECE"), QStringLiteral("myRoom"));

    QSignalSpy spy(&sampleModel, &RoomModel::dataChanged);
    const QString Id = QStringLiteral("RA151100ECE");
    const QString name = QStringLiteral("newName");
    const QString topic = QStringLiteral("myTopic");
    const QString announcement = QStringLiteral("Happy New Year announcement");
    const QString description = QStringLiteral("description");
    sampleModel.updateRoom(name, Id, topic, announcement, true, description);
    sampleWrapper = sampleModel.findRoomWrapper(Id);
    QVERIFY(sampleWrapper);

    QCOMPARE(sampleWrapper->name(), name);
    QCOMPARE(sampleWrapper->topic(), topic);
    QCOMPARE(sampleWrapper->announcement(), announcement);
    QCOMPARE(sampleWrapper->description(), description);
    QVERIFY(sampleWrapper->readOnly());
    QCOMPARE(spy.count(), 1);

    delete sampleWrapper;
    */
}

void RoomModelTest::shouldUpdateRoomFromQJsonObject()
{
    RoomModel sampleModel;
    Room *room = nullptr;
    QJsonObject roomData;

    const QString name = QStringLiteral("newName");
    const QString topic = QStringLiteral("myTopic");
    const QString announcement = QStringLiteral("Happy New Year announcement");
    roomData.insert(QStringLiteral("rid"), QJsonValue(QLatin1String("RA151100ECE")));
    roomData.insert(QStringLiteral("name"), QJsonValue(name));
    roomData.insert(QStringLiteral("announcement"), announcement);
    roomData.insert(QStringLiteral("topic"), topic);

    QCOMPARE(sampleModel.rowCount(), 0);
    sampleModel.addRoom(QStringLiteral("RA151100ECE"), QStringLiteral("myRoom"));
    QCOMPARE(sampleModel.rowCount(), 1);

    QSignalSpy spy(&sampleModel, &RoomModel::dataChanged);
    sampleModel.updateRoom(roomData);
    room = sampleModel.findRoom(QStringLiteral("RA151100ECE"));
    QVERIFY(room);

    QCOMPARE(name, room->name());
    QCOMPARE(topic, room->topic());
    QCOMPARE(announcement, room->announcement());
    QCOMPARE(room->readOnly(), false);
    QCOMPARE(spy.count(), 1);
}

void RoomModelTest::shouldUpdateSubcriptionActionRemoved()
{
    RoomModel sampleModel;
    QJsonArray input;
    QJsonObject roomData;
    roomData.insert(QStringLiteral("rid"), QJsonValue(QLatin1String("RA151100ECE")));
    input.append(QJsonValue(QLatin1String("removed")));
    input.append(QJsonValue(roomData));

    QCOMPARE(sampleModel.rowCount(), 0);
    sampleModel.addRoom(QStringLiteral("RA151100ECE"), QStringLiteral("myRoom"));
    QCOMPARE(sampleModel.rowCount(), 1);
    sampleModel.updateSubscription(input);
    QCOMPARE(sampleModel.rowCount(), 0);

    roomData.insert(QStringLiteral("rid"), QJsonValue(QLatin1String("RA151100ECE_NEW")));
    input.pop_back();
    input.append(QJsonValue(roomData));
    sampleModel.updateSubscription(input);
    QCOMPARE(sampleModel.rowCount(), 0);
}

void RoomModelTest::shouldUpdateSubcriptionActionInserted()
{
    RoomModel sampleModel;
    QJsonArray input;
    QJsonObject roomData;
    roomData.insert(QStringLiteral("rid"), QJsonValue(QLatin1String("RA151100ECE")));
    roomData.insert(QStringLiteral("name"), QJsonValue(QLatin1String("myRoom")));
    input.append(QJsonValue(QLatin1String("inserted")));
    input.append(QJsonValue(roomData));

    QCOMPARE(sampleModel.rowCount(), 0);
    sampleModel.updateSubscription(input);
    QCOMPARE(sampleModel.rowCount(), 1);

    sampleModel.updateSubscription(input);
    QCOMPARE(sampleModel.rowCount(), 1);
}

void RoomModelTest::shouldUpdateSubcriptionActionUpdated()
{
    // TODO rename autotests as it's not updatesubscrition but updateroom
    // Update subscription doesn't update topic and co
    RoomModel sampleModel;
    // QJsonArray input;
    QJsonObject roomData;
    Room *room = nullptr;

    sampleModel.addRoom(QStringLiteral("RA151100ECE"), QStringLiteral("myRoom"));

    const QString name = QStringLiteral("newName");
    const QString topic = QStringLiteral("myTopic");
    const QString announcement = QStringLiteral("Happy New Year announcement");
    roomData.insert(QStringLiteral("rid"), QJsonValue(QLatin1String("RA151100ECE")));
    roomData.insert(QStringLiteral("name"), QJsonValue(name));
    roomData.insert(QStringLiteral("announcement"), announcement);
    roomData.insert(QStringLiteral("topic"), topic);
    //    input.append(QJsonValue(QLatin1String("updated")));
    //    input.append(roomData);

    QCOMPARE(sampleModel.rowCount(), 1);
    QSignalSpy spy(&sampleModel, &RoomModel::dataChanged);
    sampleModel.updateRoom(roomData);
    QCOMPARE(sampleModel.rowCount(), 1);

    room = sampleModel.findRoom(QStringLiteral("RA151100ECE"));
    QVERIFY(room);

    QCOMPARE(name, room->name());
    QCOMPARE(topic, room->topic());
    QCOMPARE(announcement, room->announcement());
    QCOMPARE(room->readOnly(), false);
    QCOMPARE(spy.count(), 1);
}

void RoomModelTest::shouldClear()
{
    RoomModel sampleModel;
    QSignalSpy rowInsertedSpy(&sampleModel, &RoomModel::rowsInserted);
    QSignalSpy rowABTInserted(&sampleModel, &RoomModel::rowsAboutToBeInserted);
    QSignalSpy rowRemovedSpy(&sampleModel, &RoomModel::rowsRemoved);
    QSignalSpy rowABTRemoved(&sampleModel, &RoomModel::rowsAboutToBeRemoved);

    QCOMPARE(sampleModel.rowCount(), 0);
    sampleModel.addRoom(QStringLiteral("RA151100ECE"), QStringLiteral("myRoom"));
    QCOMPARE(sampleModel.rowCount(), 1);

    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,0"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,0"));

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    sampleModel.clear();

    QCOMPARE(sampleModel.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QStringLiteral("0,0"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QStringLiteral("0,0"));

    for (int i = 0; i < 15; i++) {
        sampleModel.addRoom(QStringLiteral("RA151100ECE%1").arg(i), QStringLiteral("myRoom%1").arg(i));
    }
    QCOMPARE(sampleModel.rowCount(), 15);

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    sampleModel.clear();

    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QStringLiteral("0,14"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QStringLiteral("0,14"));
}

void RoomModelTest::shouldReset()
{
    RoomModel sampleModel;
    // RoomWrapper *sampleWrapper;

    const QString Id = QStringLiteral("RA151100ECE");
    const QString name = QStringLiteral("myRoom");
    QCOMPARE(sampleModel.rowCount(), 0);
    sampleModel.addRoom(Id, name);
    QCOMPARE(sampleModel.rowCount(), 1);
    // TODO: should uncomment this after enabling cache in roomModel
    /*
        sampleModel.reset();
        QCOMPARE(1, sampleModel.rowCount());
        sampleWrapper = sampleModel.findRoomWrapper(Id);
        QCOMPARE(name, sampleWrapper->name());
        */
}

void RoomModelTest::shouldReturnDataDefault()
{
    RoomModel sampleModel;
    QVariant output;
    QString Id = QStringLiteral("RA151100ECE");
    QString name = QStringLiteral("myRoom");
    sampleModel.addRoom(Id, name);

    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomName);
    QCOMPARE(output.toString(), name);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomId);
    QCOMPARE(output.toString(), Id);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomSelected);
    QCOMPARE(output.toBool(), false);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomType); // channel type
    QCOMPARE(output.value<Room::RoomType>(), Room::RoomType::Unknown);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomOwnerUserId);
    QVERIFY(output.toString().isEmpty());
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomOwnerUserName);
    QVERIFY(output.toString().isEmpty());
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomTopic);
    QVERIFY(output.toString().isEmpty());
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomMutedUsers);
    QVERIFY(output.toStringList().isEmpty());
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomJitsiTimeout);
    QCOMPARE(output, QVariant(qint64(-1)));
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomReadOnly);
    QCOMPARE(output.toBool(), false);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomAnnouncement);
    QVERIFY(output.toString().isEmpty());
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomUnread);
    QCOMPARE(output, QVariant(int(0))); // quint64 not used in room.cpp???
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomFavorite);
    QCOMPARE(output.toBool(), false);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomOpen);
    QCOMPARE(output.toBool(), false);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomSection);
    QVERIFY(output.toString().isEmpty());
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomOrder);
    QCOMPARE(output, QVariant(int(35))); // not favorite (20), not mainTeam (10) + RoomType::Unknown (5)
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomIcon);
    QCOMPARE(output, QVariant(QIcon()));
}

void RoomModelTest::shouldReturnData()
{
    Room input(nullptr);
    const QString Id = QStringLiteral("RA151100ECE");
    const QString name = QStringLiteral("myRoom");
    const bool selected = true;
    const QString roomType = QStringLiteral("p");
    const QString userId = QStringLiteral("sdfsdfs");
    const QString userName = QStringLiteral("pp");
    const QString topic = QStringLiteral("topic");
    const QStringList mutedUsers = QStringList{QStringLiteral("mutedUsers"), QStringLiteral("muted2")};
    const qint64 time = 55;
    const bool readOnly = true;
    const QString announcement = QStringLiteral("AA");
    const int unread = 66;
    const bool favorite = true;
    const bool open = true;

    input.setRoomId(Id);
    input.setName(name);
    input.setSelected(selected);
    input.setChannelType(Room::roomTypeFromString(roomType));
    input.setRoomCreatorUserId(userId);
    input.setRoomCreatorUserName(userName);
    input.setTopic(topic);
    input.setMutedUsers(mutedUsers);
    input.setJitsiTimeout(time);
    input.setReadOnly(readOnly);
    input.setAnnouncement(announcement);
    input.setUnread(unread);
    input.setFavorite(favorite);
    input.setOpen(open);

    RoomModel sampleModel;
    QVariant output;
    QVERIFY(sampleModel.addRoom(&input)); // don't pass address. pass pointer variable

    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomName);
    QCOMPARE(output.toString(), name);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomId);
    QCOMPARE(output.toString(), Id);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomSelected);
    QCOMPARE(output.toBool(), selected);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomType); // channel type
    QCOMPARE(Room::roomFromRoomType(output.value<Room::RoomType>()), roomType);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomOwnerUserId);
    QCOMPARE(output.toString(), userId);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomOwnerUserName);
    QCOMPARE(output.toString(), userName);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomTopic);
    QCOMPARE(output.toString(), topic);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomMutedUsers);
    QCOMPARE(output.toStringList(), mutedUsers);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomJitsiTimeout);
    QCOMPARE(output, QVariant(time));
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomReadOnly);
    QCOMPARE(output.toBool(), readOnly);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomAnnouncement);
    QCOMPARE(output.toString(), announcement);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomUnread);
    QCOMPARE(output, QVariant(unread)); // quint64 not used in room.cpp???
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomFavorite);
    QCOMPARE(output.toBool(), favorite);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomOpen);
    QCOMPARE(output.toBool(), open);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomSection);
    QCOMPARE(output.value<RoomModel::Section>(), RoomModel::Section::Favorites); // first priority for favorites and then to channels
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomOrder);
    QCOMPARE(output, QVariant(int(11))); // Favorite, private room
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomIcon);
    QCOMPARE(output, QVariant(QIcon::fromTheme(QStringLiteral("lock"))));
}

void RoomModelTest::shouldInsertRoom_data()
{
    QTest::addColumn<QString>("insertRoomFileName");
    QTest::addColumn<QString>("roomId");
    QTest::newRow("insertroom1") << QStringLiteral("insertroom1") << QStringLiteral("fooid");
    QTest::newRow("insertroom2") << QStringLiteral("insertroom2") << QStringLiteral("bla1");
}

void RoomModelTest::shouldInsertRoom()
{
    QFETCH(QString, insertRoomFileName);
    QFETCH(QString, roomId);

    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/insert-rooms/") + insertRoomFileName + QLatin1String(".json");
    const QJsonObject fields = AutoTestHelper::loadJsonObject(originalJsonFile);

    RoomModel sampleModel;
    const QString generatedRoomId = sampleModel.insertRoom(fields);
    QCOMPARE(generatedRoomId, roomId);
    QCOMPARE(sampleModel.rowCount(), 1);
    Room *r = sampleModel.findRoom(generatedRoomId);
    QVERIFY(r);

    // qDebug() << " fields"<<fields;

    const QByteArray ba = Room::serialize(r, false);
    // qDebug() << " ba " << ba;
    const QJsonDocument docSerialized = QJsonDocument::fromJson(ba);

    const QByteArray jsonIndented = docSerialized.toJson(QJsonDocument::Indented);
    AutoTestHelper::compareFile(QStringLiteral("/insert-rooms/"), jsonIndented, insertRoomFileName);

    auto m = Room::fromJSon(docSerialized.object());
    QCOMPARE(*r, *m);
}

void RoomModelTest::shouldOrderRooms()
{
    // GIVEN
    RocketChatAccount account;
    account.setSortUnreadOnTop(true);
    RoomModel sampleModel(&account);
    int count = 0;
    std::vector<Room *> rooms;
    auto addRoom = [&](bool unread, bool favorite, bool mainTeam, Room::RoomType roomType, const char *name) {
        auto room = new Room;
        room->setRoomId(QString::number(count));
        QString str = QString::fromLatin1(name);
        if (favorite) {
            str.prepend(QStringLiteral("fav_"));
        }
        room->setName(unread ? (QStringLiteral("unread_") + str) : str);
        room->setFavorite(favorite);
        room->setUnread(unread ? 1 : 0);
        room->setParentRid(QStringLiteral("parentRId")); // not empty
        TeamInfo teamInfo;
        teamInfo.setMainTeam(mainTeam);
        room->setTeamInfo(teamInfo);
        room->setChannelType(roomType);
        QVERIFY(sampleModel.addRoom(room));
        rooms.push_back(room);
        ++count;
    };
    for (bool unread : {false, true}) {
        for (bool favorite : {false, true}) {
            addRoom(unread, favorite, false, Room::RoomType::Private, "p");
            addRoom(unread, favorite, false, Room::RoomType::Channel, "c");
            addRoom(unread, favorite, false, Room::RoomType::Direct, "d");
            addRoom(unread, favorite, true, Room::RoomType::Private, "mp");
            addRoom(unread, favorite, true, Room::RoomType::Channel, "mc");
            addRoom(unread, favorite, true, Room::RoomType::Direct, "md");
        }
    }
    QCOMPARE(sampleModel.rowCount(), count);

    // WHEN
    QMap<int, QString> orderedRooms;
    QMap<int, QString> orderedSections;
    for (int row = 0; row < count; ++row) {
        const QModelIndex index = sampleModel.index(row, 0);
        const QString name = index.data(RoomModel::RoomName).toString();
        const int order = index.data(RoomModel::RoomOrder).toInt();
        QVERIFY(!orderedRooms.contains(order));
        orderedRooms.insert(order, name);
        const auto section = index.data(RoomModel::RoomSection).value<RoomModel::Section>();
        orderedSections.insert(order, RoomModel::sectionName(section));
    }

    // THEN
    const QStringList expectedRoomNames{
        // Unread
        QStringLiteral("unread_fav_mc"),
        QStringLiteral("unread_fav_md"),
        QStringLiteral("unread_fav_mp"),
        QStringLiteral("unread_fav_c"),
        QStringLiteral("unread_fav_d"),
        QStringLiteral("unread_fav_p"),
        QStringLiteral("unread_mc"),
        QStringLiteral("unread_md"),
        QStringLiteral("unread_mp"),
        QStringLiteral("unread_c"),
        QStringLiteral("unread_d"),
        QStringLiteral("unread_p"),
        // Favorites
        QStringLiteral("fav_mc"),
        QStringLiteral("fav_md"),
        QStringLiteral("fav_mp"),
        QStringLiteral("fav_c"),
        QStringLiteral("fav_d"),
        QStringLiteral("fav_p"),
        // Main team
        QStringLiteral("mc"),
        QStringLiteral("md"),
        QStringLiteral("mp"),
        // Read channel/discussion/private messages
        QStringLiteral("c"),
        QStringLiteral("d"),
        QStringLiteral("p"),
    };
    QCOMPARE(orderedRooms.values(), expectedRoomNames);

    QStringList expectedSections;
    expectedSections.reserve(expectedRoomNames.size());
    for (int i = 0; i < 12; ++i) {
        expectedSections.append(QStringLiteral("Unread"));
    }
    for (int i = 0; i < 6; ++i) {
        expectedSections.append(QStringLiteral("Favorites"));
    }
    for (int i = 0; i < 3; ++i) {
        expectedSections.append(QStringLiteral("Teams"));
    }
    expectedSections.append(QStringLiteral("Rooms"));
    expectedSections.append(QStringLiteral("Private Messages"));
    expectedSections.append(QStringLiteral("Discussions"));
    QCOMPARE(orderedSections.values(), expectedSections);

    qDeleteAll(rooms);
}

// TODO add autotest for notification update.
