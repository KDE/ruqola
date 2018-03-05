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
#include "model/usersforroommodel.h"
#include "model/usersforroomfilterproxymodel.h"
#include "model/filesforroomfilterproxymodel.h"
#include "model/filesforroommodel.h"
#include <QJsonDocument>
#include <QTest>
#include <QSignalSpy>
#include <QProcess>

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
    QCOMPARE(input.userMentions(), -1);
    QCOMPARE(input.unread(), -1);
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
    input.setArchived(true);
    input.setDescription(QStringLiteral("dd"));
    input.setUserMentions(3);
    const QByteArray ba = Room::serialize(&input);
    Room *output = Room::fromJSon(QJsonObject(QJsonDocument::fromBinaryData(ba).object()));
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
    input.setArchived(true);
    input.setDescription(QStringLiteral("ddd"));

    QCOMPARE(spyNameChanged.count(), 1);
    QCOMPARE(spyannouncementChanged.count(), 1);
    QCOMPARE(spytopicChanged.count(), 1);
    QCOMPARE(spyfavoriteChanged.count(), 1);
    QCOMPARE(spyalertChanged.count(), 1);
    QCOMPARE(spyreadOnlyChanged.count(), 1);
    QCOMPARE(spyunreadChanged.count(), 1);
    QCOMPARE(spyblockerChanged.count(), 1);
    QCOMPARE(spyarchivedChanged.count(), 1);
    QCOMPARE(spydescriptionChanged.count(), 1);
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

void RoomTest::compareFile(const QByteArray &data, const QString &name)
{
    const QString refFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/room/") + name + QStringLiteral(".ref");
    const QString generatedFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/room/") + name + QStringLiteral("-generated.ref");
    //Create generated file
    QFile f(generatedFile);
    QVERIFY(f.open(QIODevice::WriteOnly | QIODevice::Truncate));
    f.write(data);
    f.close();

    // compare to reference file
    QStringList args = QStringList()
                       << QStringLiteral("-u")
                       << refFile
                       << generatedFile;
    QProcess proc;
    proc.setProcessChannelMode(QProcess::ForwardedChannels);
    proc.start(QStringLiteral("diff"), args);
    QVERIFY(proc.waitForFinished());
    QCOMPARE(proc.exitCode(), 0);
}

void RoomTest::shouldParseRoom_data()
{
    QTest::addColumn<QString>("fileName");
    //Missing _updatedAt/ts/_id/groupMentions/ls/roles (implement roles ! )
    QTest::newRow("notification-room") << QStringLiteral("notification-room");

    QTest::newRow("unread-usermentions-room") << QStringLiteral("unread-usermentions-room");
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
    compareFile(jsonIndented, fileName);

    Room *m = Room::fromJSon(docSerialized.object());
    QCOMPARE(r, *m);
    delete m;
}

void RoomTest::shouldParseRoomAndUpdate_data()
{
    QTest::addColumn<QString>("fileNameinit");
    QTest::addColumn<QString>("fileNameupdate");
    //Missing _updatedAt/ts/_id/groupMentions/ls/roles (implement roles ! )
    QTest::newRow("notification-room") << QStringLiteral("notification-room") << QStringLiteral("notification-roomupdate");

}

void RoomTest::shouldParseRoomAndUpdate()
{
    //TODO
    QFETCH(QString, fileNameinit);
    QFETCH(QString, fileNameupdate);

    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/room/") + fileNameinit + QStringLiteral(".json");
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
    compareFile(jsonIndented, fileNameinit);

    Room *m = Room::fromJSon(docSerialized.object());
    QCOMPARE(r, *m);
    delete m;
}

//TODO add more autotests signal and co.
// add parseUpdateRoom feature
