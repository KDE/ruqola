/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "roomwrappertest.h"
#include "roomwrapper.h"
#include "room.h"
#include <QTest>
#include <QSignalSpy>
QTEST_GUILESS_MAIN(RoomWrapperTest)

RoomWrapperTest::RoomWrapperTest(QObject *parent)
    : QObject(parent)
{

}

void RoomWrapperTest::shouldHaveDefaultValue()
{
    Room *room = new Room();
    RoomWrapper w(room);
    QVERIFY(w.announcement().isEmpty());
    QVERIFY(w.name().isEmpty());
    QVERIFY(w.topic().isEmpty());
    QVERIFY(w.channelType().isEmpty());
    QCOMPARE(w.favorite(), false);
    QCOMPARE(w.blocker(), false);
    QVERIFY(w.userId().isEmpty());

    delete room;
}

void RoomWrapperTest::shouldAssignValue()
{
    Room *room = new Room();
    RoomWrapper w(room);
    QSignalSpy spyRoomAnnoucementChanged(&w, &RoomWrapper::announcementChanged);
    QSignalSpy spyRoomTopicChanged(&w, &RoomWrapper::topicChanged);
    QSignalSpy spyRoomNameChanged(&w, &RoomWrapper::nameChanged);
    QSignalSpy spyRoomFavoriteChanged(&w, &RoomWrapper::favoriteChanged);
    QSignalSpy spyRoomBlockerChanged(&w, &RoomWrapper::blockerChanged);

    room->setAnnouncement(QStringLiteral("foo"));

    QCOMPARE(spyRoomAnnoucementChanged.count(), 1);
    QCOMPARE(spyRoomTopicChanged.count(), 0);
    QCOMPARE(spyRoomNameChanged.count(), 0);
    QCOMPARE(spyRoomFavoriteChanged.count(), 0);
    QCOMPARE(spyRoomBlockerChanged.count(), 0);

    spyRoomAnnoucementChanged.clear();
    spyRoomTopicChanged.clear();
    spyRoomNameChanged.clear();
    spyRoomFavoriteChanged.clear();
    spyRoomBlockerChanged.clear();

    room->setAnnouncement(QStringLiteral("foo"));
    QCOMPARE(spyRoomAnnoucementChanged.count(), 0);
    QCOMPARE(spyRoomTopicChanged.count(), 0);
    QCOMPARE(spyRoomNameChanged.count(), 0);
    QCOMPARE(spyRoomFavoriteChanged.count(), 0);
    QCOMPARE(spyRoomBlockerChanged.count(), 0);

    spyRoomAnnoucementChanged.clear();
    spyRoomTopicChanged.clear();
    spyRoomNameChanged.clear();
    spyRoomFavoriteChanged.clear();
    spyRoomBlockerChanged.clear();

    room->setTopic(QStringLiteral("foo"));
    QCOMPARE(spyRoomAnnoucementChanged.count(), 0);
    QCOMPARE(spyRoomTopicChanged.count(), 1);
    QCOMPARE(spyRoomNameChanged.count(), 0);
    QCOMPARE(spyRoomFavoriteChanged.count(), 0);
    QCOMPARE(spyRoomBlockerChanged.count(), 0);

    spyRoomAnnoucementChanged.clear();
    spyRoomTopicChanged.clear();
    spyRoomNameChanged.clear();
    spyRoomFavoriteChanged.clear();
    spyRoomBlockerChanged.clear();

    room->setTopic(QStringLiteral("foo"));
    QCOMPARE(spyRoomAnnoucementChanged.count(), 0);
    QCOMPARE(spyRoomTopicChanged.count(), 0);
    QCOMPARE(spyRoomNameChanged.count(), 0);
    QCOMPARE(spyRoomFavoriteChanged.count(), 0);
    QCOMPARE(spyRoomBlockerChanged.count(), 0);

    spyRoomAnnoucementChanged.clear();
    spyRoomTopicChanged.clear();
    spyRoomNameChanged.clear();
    spyRoomFavoriteChanged.clear();
    spyRoomBlockerChanged.clear();

    //favorite
    room->setFavorite(true);
    QCOMPARE(spyRoomAnnoucementChanged.count(), 0);
    QCOMPARE(spyRoomTopicChanged.count(), 0);
    QCOMPARE(spyRoomNameChanged.count(), 0);
    QCOMPARE(spyRoomFavoriteChanged.count(), 1);
    QCOMPARE(spyRoomBlockerChanged.count(), 0);

    spyRoomAnnoucementChanged.clear();
    spyRoomTopicChanged.clear();
    spyRoomNameChanged.clear();
    spyRoomFavoriteChanged.clear();
    spyRoomBlockerChanged.clear();

    room->setFavorite(true);
    QCOMPARE(spyRoomAnnoucementChanged.count(), 0);
    QCOMPARE(spyRoomTopicChanged.count(), 0);
    QCOMPARE(spyRoomNameChanged.count(), 0);
    QCOMPARE(spyRoomFavoriteChanged.count(), 0);
    QCOMPARE(spyRoomBlockerChanged.count(), 0);

    spyRoomAnnoucementChanged.clear();
    spyRoomTopicChanged.clear();
    spyRoomNameChanged.clear();
    spyRoomFavoriteChanged.clear();
    spyRoomBlockerChanged.clear();

    //blocker
    room->setBlocker(true);
    QCOMPARE(spyRoomAnnoucementChanged.count(), 0);
    QCOMPARE(spyRoomTopicChanged.count(), 0);
    QCOMPARE(spyRoomNameChanged.count(), 0);
    QCOMPARE(spyRoomFavoriteChanged.count(), 0);
    QCOMPARE(spyRoomBlockerChanged.count(), 1);

    spyRoomAnnoucementChanged.clear();
    spyRoomTopicChanged.clear();
    spyRoomNameChanged.clear();
    spyRoomFavoriteChanged.clear();
    spyRoomBlockerChanged.clear();

    room->setBlocker(true);
    QCOMPARE(spyRoomAnnoucementChanged.count(), 0);
    QCOMPARE(spyRoomTopicChanged.count(), 0);
    QCOMPARE(spyRoomNameChanged.count(), 0);
    QCOMPARE(spyRoomFavoriteChanged.count(), 0);
    QCOMPARE(spyRoomBlockerChanged.count(), 0);

    spyRoomAnnoucementChanged.clear();
    spyRoomTopicChanged.clear();
    spyRoomNameChanged.clear();
    spyRoomFavoriteChanged.clear();
    spyRoomBlockerChanged.clear();
    room->setBlocker(false);
    QCOMPARE(spyRoomAnnoucementChanged.count(), 0);
    QCOMPARE(spyRoomTopicChanged.count(), 0);
    QCOMPARE(spyRoomNameChanged.count(), 0);
    QCOMPARE(spyRoomFavoriteChanged.count(), 0);
    QCOMPARE(spyRoomBlockerChanged.count(), 1);

    spyRoomAnnoucementChanged.clear();
    spyRoomTopicChanged.clear();
    spyRoomNameChanged.clear();
    spyRoomFavoriteChanged.clear();
    spyRoomBlockerChanged.clear();

    const QString userId = QStringLiteral("foo");
    room->setUserId(userId);
    QCOMPARE(spyRoomAnnoucementChanged.count(), 0);
    QCOMPARE(spyRoomTopicChanged.count(), 0);
    QCOMPARE(spyRoomNameChanged.count(), 0);
    QCOMPARE(spyRoomFavoriteChanged.count(), 0);
    QCOMPARE(spyRoomBlockerChanged.count(), 0);
    QCOMPARE(w.userId(), userId);

    delete room;
}
