/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channellistviewtest.h"
using namespace Qt::Literals::StringLiterals;

#include "channellist/channellistview.h"
#include "model/roomfilterproxymodel.h"
#include "model/roommodel.h"
#include "ownuser/ownuserpreferences.h"
#include "rocketchataccount.h"
#include "room.h"
#include <QSignalSpy>
#include <QTest>
#include <memory>

QTEST_MAIN(ChannelListViewTest)
ChannelListViewTest::ChannelListViewTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelListViewTest::shouldHaveDefaultValues()
{
    const ChannelListView w;
    QCOMPARE(w.horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
    QCOMPARE(w.itemDelegate()->objectName(), u"mChannelListDelegate"_s);
    QCOMPARE(w.isHeaderHidden(), true);
    QCOMPARE(w.rootIsDecorated(), false);
    QCOMPARE(w.uniformRowHeights(), false);
    QCOMPARE(w.itemsExpandable(), false);
    QCOMPARE(w.indentation(), 0);
    QVERIFY(w.filterModel());
    QCOMPARE(w.filterModel()->objectName(), u"mRoomFilterProxyModel"_s);
}

void ChannelListViewTest::shouldKeepSectionsExpandedOnUnreadOnTopChanges()
{
    // GIVEN one unread room and one read room, with "unread on top" enabled
    RocketChatAccount account(u"account"_s);
    OwnUserPreferences ownUserPreferences;
    ownUserPreferences.setShowUnread(true);
    account.setOwnUserPreferences(ownUserPreferences);

    std::vector<std::unique_ptr<Room>> rooms;
    for (int i = 1; i <= 2; ++i) {
        auto room = std::make_unique<Room>(&account);
        room->setRoomId(QByteArray("room") + QByteArray::number(i));
        room->setName(QStringLiteral("Room %1").arg(i));
        room->setChannelType(Room::RoomType::Channel);
        room->setOpen(true);
        if (i == 1) {
            room->setUnread(1);
        }
        QVERIFY(account.roomModel()->addRoom(room.get()));
        rooms.push_back(std::move(room));
    }

    ChannelListView view;
    view.setCurrentRocketChatAccount(&account);
    auto *const proxy = view.filterModel();
    QCOMPARE(proxy->rowCount(), 2); // Unread and Rooms

    // WHEN disabling and re-enabling "unread on top"
    ownUserPreferences.setShowUnread(false);
    account.setOwnUserPreferences(ownUserPreferences);
    Q_EMIT account.ownUserUiPreferencesChanged();
    QCOMPARE(proxy->rowCount(), 1); // Rooms

    ownUserPreferences.setShowUnread(true);
    account.setOwnUserPreferences(ownUserPreferences);
    Q_EMIT account.ownUserUiPreferencesChanged();

    // THEN the sections are back and still expanded, the source model only emits moves so the
    // proxy reports them with layoutChanged rather than rowsInserted
    QCOMPARE(proxy->rowCount(), 2);
    for (int row = 0; row < proxy->rowCount(); ++row) {
        const QModelIndex section = proxy->index(row, 0);
        QCOMPARE(proxy->rowCount(section), 1);
        QVERIFY2(view.isExpanded(section), qPrintable(section.data().toString() + u" is collapsed"_s));
    }
}

void ChannelListViewTest::shouldKeepSectionsExpandedAfterClearingFilter()
{
    // GIVEN one room
    RocketChatAccount account(u"account"_s);
    const auto room = std::make_unique<Room>(&account);
    room->setRoomId("room1");
    room->setName(u"Room 1"_s);
    room->setChannelType(Room::RoomType::Channel);
    room->setOpen(true);
    QVERIFY(account.roomModel()->addRoom(room.get()));

    ChannelListView view;
    view.setCurrentRocketChatAccount(&account);
    auto *const proxy = view.filterModel();
    QCOMPARE(proxy->rowCount(), 1);

    // WHEN filtering everything out and then clearing the filter again
    proxy->setFilterString(u"zz"_s);
    QCOMPARE(proxy->rowCount(), 0);
    proxy->setFilterString(QString());

    // THEN the section is back and expanded, otherwise the room list looks empty
    QCOMPARE(proxy->rowCount(), 1);
    const QModelIndex section = proxy->index(0, 0);
    QCOMPARE(proxy->rowCount(section), 1);
    QVERIFY(view.isExpanded(section));
}

void ChannelListViewTest::shouldNotSelectRoomWhileFiltering()
{
    // GIVEN two rooms, with the first one current
    RocketChatAccount account(u"account"_s);
    // Run the queued RocketChatAccount::clearModels() now, it would delete the rooms during wait() below
    QCoreApplication::processEvents();
    std::vector<std::unique_ptr<Room>> rooms;
    for (int i = 1; i <= 2; ++i) {
        auto room = std::make_unique<Room>(&account);
        room->setRoomId(QByteArray("room") + QByteArray::number(i));
        room->setName(QStringLiteral("Room %1").arg(i));
        room->setChannelType(Room::RoomType::Channel);
        room->setOpen(true);
        QVERIFY(account.roomModel()->addRoom(room.get()));
        rooms.push_back(std::move(room));
    }

    ChannelListView view;
    view.setCurrentRocketChatAccount(&account);
    auto *const proxy = view.filterModel();
    const QModelIndex section = proxy->index(0, 0);
    QCOMPARE(proxy->rowCount(section), 2);
    view.setCurrentIndex(proxy->index(0, 0, section));

    QSignalSpy selectedSpy(&view, &ChannelListView::roomSelected);
    selectedSpy.clear();

    // WHEN a filter removes the current row, so the view moves the current index
    proxy->setFilterString(u"Room 2"_s);

    // THEN opening the room must not happen from inside the proxy's removal signals, it would
    // change the room model while QSortFilterProxyModel is still walking its mapping
    QCOMPARE(selectedSpy.count(), 0);
    QVERIFY(selectedSpy.wait(500));
}

#include "moc_channellistviewtest.cpp"
