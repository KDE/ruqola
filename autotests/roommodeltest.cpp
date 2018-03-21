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

#include "room.h"
#include "roomwrapper.h"
#include "roommodeltest.h"
#include "test_model_helpers.h"
#include "model/roommodel.h"
#include "ruqola_autotest_helper.h"

#include <qglobal.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QSignalSpy>
#include <QTest>
#include <QJsonDocument>

QTEST_GUILESS_MAIN(RoomModelTest)

//TEST signal/slot

RoomModelTest::RoomModelTest(QObject *parent)
    : QObject(parent)
{
}

void RoomModelTest::shouldHaveDefaultValues()
{
    RoomModel sampleModel;
    QCOMPARE(sampleModel.rowCount(), 0);
    QHash<int, QByteArray> roles;
    roles[RoomModel::RoomName] = QByteArrayLiteral("name");
    roles[RoomModel::RoomID] = QByteArrayLiteral("room_id");
    roles[RoomModel::RoomSelected] = QByteArrayLiteral("selected");
    roles[RoomModel::RoomUnread] = QByteArrayLiteral("unread");
    roles[RoomModel::RoomType] = QByteArrayLiteral("type");
    roles[RoomModel::RoomUserName] = QByteArrayLiteral("username");
    roles[RoomModel::RoomUserID] = QByteArrayLiteral("userID");
    roles[RoomModel::RoomTopic] = QByteArrayLiteral("topic");
    roles[RoomModel::RoomMutedUsers] = QByteArrayLiteral("mutedUsers");
    roles[RoomModel::RoomJitsiTimeout] = QByteArrayLiteral("jitsiTimeout");
    roles[RoomModel::RoomRo] = QByteArrayLiteral("readOnly");
    roles[RoomModel::RoomAnnoucement] = QByteArrayLiteral("announcement");
    roles[RoomModel::RoomOpen] = QByteArrayLiteral("open");
    roles[RoomModel::RoomAlert] = QByteArrayLiteral("alert");
    roles[RoomModel::RoomOrder] = QByteArrayLiteral("roomorder");
    roles[RoomModel::RoomFavorite] = QByteArrayLiteral("favorite");
    roles[RoomModel::RoomSection] = QByteArrayLiteral("sectionname");
    roles[RoomModel::RoomIcon] = QByteArrayLiteral("channelicon");
    roles[RoomModel::RoomUserMentions] = QByteArrayLiteral("userMentions");
    QCOMPARE(sampleModel.roleNames(), roles);
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

    //Fix it. We remove + readd after that ! it's not optimal
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
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
    RoomWrapper *sampleWrapper = nullptr;

    sampleModel.addRoom(QStringLiteral("RA15"), QStringLiteral("master"));
    sampleWrapper = sampleModel.findRoomWrapper(QStringLiteral("RA151100ECE"));
    QVERIFY(!sampleWrapper);

    sampleModel.addRoom(QStringLiteral("RA151100ECE"), QStringLiteral("myRoom"));
    sampleWrapper = sampleModel.findRoomWrapper(QStringLiteral("RA151100ECE"));
    QVERIFY(sampleWrapper);
    QCOMPARE(QStringLiteral("myRoom"), sampleWrapper->name());

    delete sampleWrapper;
}

void RoomModelTest::shouldAddRoom()
{
    RoomModel sampleModel;
    RoomWrapper *sampleWrapper = nullptr;

    QCOMPARE(sampleModel.rowCount(), 0);
    sampleModel.addRoom(QStringLiteral("RA151100ECE"), QStringLiteral("myRoom"));
    QCOMPARE(sampleModel.rowCount(), 1);

    sampleWrapper = sampleModel.findRoomWrapper(QStringLiteral("RA151100ECE"));
    QVERIFY(sampleWrapper);
    QCOMPARE(QStringLiteral("myRoom"), sampleWrapper->name());

    delete sampleWrapper;
}

void RoomModelTest::shouldUpdateRoom()
{
    //FIXME!!
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
    RoomWrapper *sampleWrapper = nullptr;
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
    sampleWrapper = sampleModel.findRoomWrapper(QStringLiteral("RA151100ECE"));
    QVERIFY(sampleWrapper);

    QCOMPARE(name, sampleWrapper->name());
    QCOMPARE(topic, sampleWrapper->topic());
    QCOMPARE(announcement, sampleWrapper->announcement());
    QCOMPARE(sampleWrapper->readOnly(), false);
    QCOMPARE(spy.count(), 1);

    delete sampleWrapper;
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
    //TODO rename autotests as it's not updatesubscrition but updateroom
    //Update subscription doesn't update topic and co
    RoomModel sampleModel;
    //QJsonArray input;
    QJsonObject roomData;
    RoomWrapper *sampleWrapper = nullptr;

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

    sampleWrapper = sampleModel.findRoomWrapper(QStringLiteral("RA151100ECE"));
    QVERIFY(sampleWrapper);

    QCOMPARE(name, sampleWrapper->name());
    QCOMPARE(topic, sampleWrapper->topic());
    QCOMPARE(announcement, sampleWrapper->announcement());
    QCOMPARE(sampleWrapper->readOnly(), false);
    QCOMPARE(spy.count(), 1);

    delete sampleWrapper;
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
    //RoomWrapper *sampleWrapper;

    const QString Id = QStringLiteral("RA151100ECE");
    const QString name = QStringLiteral("myRoom");
    QCOMPARE(sampleModel.rowCount(), 0);
    sampleModel.addRoom(Id, name);
    QCOMPARE(sampleModel.rowCount(), 1);
    //TODO: should uncomment this after enabling cache in roomModel
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
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomID);
    QCOMPARE(output.toString(), Id);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomSelected);
    QCOMPARE(output.toBool(), false);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomType); //channel type
    QVERIFY(output.toString().isEmpty());
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomUserID);
    QVERIFY(output.toString().isEmpty());
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomUserName);
    QVERIFY(output.toString().isEmpty());
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomTopic);
    QVERIFY(output.toString().isEmpty());
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomMutedUsers);
    QVERIFY(output.toStringList().isEmpty());
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomJitsiTimeout);
    QCOMPARE(output, QVariant(qint64(-1)));
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomRo);
    QCOMPARE(output.toBool(), false);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomAnnoucement);
    QVERIFY(output.toString().isEmpty());
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomUnread);
    QCOMPARE(output, QVariant(int(0))); //quint64 not used in room.cpp???
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomFavorite);
    QCOMPARE(output.toBool(), false);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomOpen);
    QCOMPARE(output.toBool(), false);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomSection);
    QVERIFY(output.toString().isEmpty());
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomOrder);
    QCOMPARE(output, QVariant(int(6))); // not favorite (3) + no channel selected or 'p' (3) = total order(6)
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomIcon);
    QCOMPARE(output, QVariant(QIcon()));
}

void RoomModelTest::shouldReturnData()
{
    Room *input = new Room(nullptr);
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

    input->setRoomId(Id);
    input->setName(name);
    input->setSelected(selected);
    input->setChannelType(roomType);
    input->setRoomCreatorUserId(userId);
    input->setRoomCreatorUserName(userName);
    input->setTopic(topic);
    input->setMutedUsers(mutedUsers);
    input->setJitsiTimeout(time);
    input->setReadOnly(readOnly);
    input->setAnnouncement(announcement);
    input->setUnread(unread);
    input->setFavorite(favorite);
    input->setOpen(open);

    RoomModel sampleModel;
    QVariant output;
    sampleModel.addRoom(input);//don't pass address. pass pointer variable

    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomName);
    QCOMPARE(output.toString(), name);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomID);
    QCOMPARE(output.toString(), Id);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomSelected);
    QCOMPARE(output.toBool(), selected);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomType); //channel type
    QCOMPARE(output.toString(), roomType);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomUserID);
    QCOMPARE(output.toString(), userId);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomUserName);
    QCOMPARE(output.toString(), userName);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomTopic);
    QCOMPARE(output.toString(), topic);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomMutedUsers);
    QCOMPARE(output.toStringList(), mutedUsers);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomJitsiTimeout);
    QCOMPARE(output, QVariant(time));
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomRo);
    QCOMPARE(output.toBool(), readOnly);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomAnnoucement);
    QCOMPARE(output.toString(), announcement);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomUnread);
    QCOMPARE(output, QVariant(unread)); //quint64 not used in room.cpp???
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomFavorite);
    QCOMPARE(output.toBool(), favorite);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomOpen);
    QCOMPARE(output.toBool(), open);
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomSection);
    QCOMPARE(output.toString(), QStringLiteral("Favorites")); //first priority for favrites and then to channels
    output = sampleModel.data(sampleModel.index(0), RoomModel::RoomOrder);
    QCOMPARE(output, QVariant(int(1))); // Private room
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

    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/insert-rooms/") + insertRoomFileName + QStringLiteral(".json");
    QFile f(originalJsonFile);
    QVERIFY(f.open(QIODevice::ReadOnly));
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject fields = doc.object();

    RoomModel sampleModel;
    const QString generatedRoomId = sampleModel.insertRoom(fields);
    QCOMPARE(generatedRoomId, roomId);
    QCOMPARE(sampleModel.rowCount(), 1);
    Room *r = sampleModel.findRoom(generatedRoomId);
    QVERIFY(r);

    //qDebug() << " fields"<<fields;

    const QByteArray ba = Room::serialize(r, false);
    //qDebug() << " ba " << ba;
    const QJsonDocument docSerialized = QJsonDocument::fromJson(ba);

    const QByteArray jsonIndented = docSerialized.toJson(QJsonDocument::Indented);
    compareFile(QStringLiteral("/insert-rooms/"), jsonIndented, insertRoomFileName);

    Room *m = Room::fromJSon(docSerialized.object());
    QCOMPARE(*r, *m);
    delete m;
}

//TODO add autotest for notification update.
