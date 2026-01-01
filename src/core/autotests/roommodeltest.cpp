/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

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
#include <QStandardPaths>

#include "rocketchataccount.h"
#include <QTest>
#include <qglobal.h>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(RoomModelTest)

// TEST signal/slot

RoomModelTest::RoomModelTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
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

    sampleModel.addRoom("myRoomID1"_ba, u"myRoom1"_s);
    QCOMPARE(sampleModel.rowCount(), 1);

    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), u"0,0"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), u"0,0"_s);

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    sampleModel.addRoom("myRoomID1"_ba, u"bla bla"_s);
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

    sampleModel.addRoom("myRoomID2"_ba, u"myRoom2"_s, true);
    QCOMPARE(sampleModel.rowCount(), 2);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), u"1,1"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), u"1,1"_s);
}

void RoomModelTest::shouldFindRoom()
{
    RoomModel sampleModel;
    Room *room = nullptr;

    sampleModel.addRoom("RA15"_ba, u"master"_s);
    room = sampleModel.findRoom("RA151100ECE"_ba);
    QVERIFY(!room);

    sampleModel.addRoom("RA151100ECE"_ba, u"myRoom"_s);
    room = sampleModel.findRoom("RA151100ECE"_ba);
    QVERIFY(room);
    QCOMPARE(u"myRoom"_s, room->name());
}

void RoomModelTest::shouldAddRoom()
{
    RoomModel sampleModel;
    Room *room = nullptr;

    QCOMPARE(sampleModel.rowCount(), 0);
    sampleModel.addRoom("RA151100ECE"_ba, u"myRoom"_s);
    QCOMPARE(sampleModel.rowCount(), 1);

    room = sampleModel.findRoom("RA151100ECE"_ba);
    QVERIFY(room);
    QCOMPARE(u"myRoom"_s, room->name());
}

void RoomModelTest::shouldUpdateRoom()
{
    // FIXME!!
    /*
    RoomModel sampleModel;
    RoomWrapper *sampleWrapper = nullptr;

    sampleModel.addRoom(u"RA151100ECE"_s, u"myRoom"_s);

    QSignalSpy spy(&sampleModel, &RoomModel::dataChanged);
    const QString Id = u"RA151100ECE"_s;
    const QString name = u"newName"_s;
    const QString topic = u"myTopic"_s;
    const QString announcement = u"Happy New Year announcement"_s;
    const QString description = u"description"_s;
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

    const QString name = u"newName"_s;
    const QString topic = u"myTopic"_s;
    const QString announcement = u"Happy New Year announcement"_s;
    roomData.insert(u"rid"_s, QJsonValue("RA151100ECE"_L1));
    roomData.insert(u"name"_s, QJsonValue(name));
    roomData.insert(u"announcement"_s, announcement);
    roomData.insert(u"topic"_s, topic);

    QCOMPARE(sampleModel.rowCount(), 0);
    sampleModel.addRoom("RA151100ECE"_ba, u"myRoom"_s);
    QCOMPARE(sampleModel.rowCount(), 1);

    QSignalSpy spy(&sampleModel, &RoomModel::dataChanged);
    sampleModel.updateRoom(roomData);
    room = sampleModel.findRoom("RA151100ECE"_ba);
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
    roomData.insert(u"rid"_s, QJsonValue("RA151100ECE"_L1));
    input.append(QJsonValue("removed"_L1));
    input.append(QJsonValue(roomData));

    QCOMPARE(sampleModel.rowCount(), 0);
    sampleModel.addRoom("RA151100ECE"_ba, u"myRoom"_s);
    QCOMPARE(sampleModel.rowCount(), 1);
    sampleModel.updateSubscription(input);
    QCOMPARE(sampleModel.rowCount(), 0);

    roomData.insert(u"rid"_s, QJsonValue("RA151100ECE_NEW"_L1));
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
    roomData.insert(u"rid"_s, QJsonValue("RA151100ECE"_L1));
    roomData.insert(u"name"_s, QJsonValue("myRoom"_L1));
    input.append(QJsonValue("inserted"_L1));
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

    sampleModel.addRoom("RA151100ECE"_ba, u"myRoom"_s);

    const QString name = u"newName"_s;
    const QString topic = u"myTopic"_s;
    const QString announcement = u"Happy New Year announcement"_s;
    roomData.insert(u"rid"_s, QJsonValue("RA151100ECE"_L1));
    roomData.insert(u"name"_s, QJsonValue(name));
    roomData.insert(u"announcement"_s, announcement);
    roomData.insert(u"topic"_s, topic);
    //    input.append(QJsonValue("updated"_L1));
    //    input.append(roomData);

    QCOMPARE(sampleModel.rowCount(), 1);
    QSignalSpy spy(&sampleModel, &RoomModel::dataChanged);
    sampleModel.updateRoom(roomData);
    QCOMPARE(sampleModel.rowCount(), 1);

    room = sampleModel.findRoom("RA151100ECE"_ba);
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

    QCOMPARE(sampleModel.rowCount(), 0);
    sampleModel.addRoom("RA151100ECE"_ba, u"myRoom"_s);
    QCOMPARE(sampleModel.rowCount(), 1);

    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), u"0,0"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), u"0,0"_s);

    rowInsertedSpy.clear();
    rowABTInserted.clear();

    sampleModel.clear();

    QCOMPARE(sampleModel.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);

    for (int i = 0; i < 15; i++) {
        sampleModel.addRoom(QByteArray("RA151100ECE%1") + QByteArray::number(i), u"myRoom%1"_s.arg(i));
    }
    QCOMPARE(sampleModel.rowCount(), 15);

    rowInsertedSpy.clear();
    rowABTInserted.clear();

    sampleModel.clear();

    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
}

void RoomModelTest::shouldReset()
{
    RoomModel sampleModel;
    // RoomWrapper *sampleWrapper;

    const QByteArray Id = "RA151100ECE"_ba;
    const QString name = u"myRoom"_s;
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
    QByteArray Id("RA151100ECE");
    QString name = u"myRoom"_s;
    sampleModel.addRoom(Id, name);

    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomName);
    QCOMPARE(output.toString(), name);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomId);
    QCOMPARE(output.toByteArray(), Id);
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
    QCOMPARE(output.value<RoomModel::Section>(), RoomModel::Section::Unknown);
}

void RoomModelTest::shouldReturnData()
{
    Room input(nullptr);
    const QByteArray Id("RA151100ECE");
    const QString name = u"myRoom"_s;
    const bool selected = true;
    const QString roomType = u"p"_s;
    const QByteArray userId = "sdfsdfs"_ba;
    const QString userName = u"pp"_s;
    const QString topic = u"topic"_s;
    const QStringList mutedUsers = QStringList{u"mutedUsers"_s, u"muted2"_s};
    const qint64 time = 55;
    const bool readOnly = true;
    const QString announcement = u"AA"_s;
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
    QCOMPARE(output.toByteArray(), Id);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomSelected);
    QCOMPARE(output.toBool(), selected);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomType); // channel type
    QCOMPARE(Room::roomFromRoomType(output.value<Room::RoomType>()), roomType);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomOwnerUserId);
    QCOMPARE(output.toByteArray(), userId);
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
    // output = sampleModel.data(sampleModel.index(0), RoomModel::RoomIcon);
    // QCOMPARE(output, QVariant(QIcon::fromTheme(u"lock"_s)));
}

void RoomModelTest::shouldInsertRoom_data()
{
    QTest::addColumn<QString>("insertRoomFileName");
    QTest::addColumn<QByteArray>("roomId");
    QTest::newRow("insertroom1") << u"insertroom1"_s << "fooid"_ba;
    QTest::newRow("insertroom2") << u"insertroom2"_s << "bla1"_ba;
}

void RoomModelTest::shouldInsertRoom()
{
    QFETCH(QString, insertRoomFileName);
    QFETCH(QByteArray, roomId);

    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/insert-rooms/"_L1 + insertRoomFileName + ".json"_L1;
    const QJsonObject fields = AutoTestHelper::loadJsonObject(originalJsonFile);

    RoomModel sampleModel;
    const QByteArray generatedRoomId = sampleModel.insertRoom(fields);
    QCOMPARE(generatedRoomId, roomId);
    QCOMPARE(sampleModel.rowCount(), 1);
    Room *r = sampleModel.findRoom(generatedRoomId);
    QVERIFY(r);

    // qDebug() << " fields"<<fields;

    const QByteArray ba = Room::serialize(r, false);
    // qDebug() << " ba " << ba;
    const QJsonDocument docSerialized = QJsonDocument::fromJson(ba);

    const QByteArray jsonIndented = docSerialized.toJson(QJsonDocument::Indented);
    AutoTestHelper::compareFile(u"/insert-rooms/"_s, jsonIndented, insertRoomFileName);

    auto m = Room::deserialize(docSerialized.object());
    QCOMPARE(*r, *m);
}

// TODO add autotest for notification update.

#include "moc_roommodeltest.cpp"
