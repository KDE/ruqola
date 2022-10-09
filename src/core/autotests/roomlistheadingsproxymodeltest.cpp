/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "roomlistheadingsproxymodeltest.h"
#include "model/roomfilterproxymodel.h"
#include "model/roomlistheadingsproxymodel.h"

#include <QStandardPaths>
#include <QTest>
#include <rocketchataccount.h>

QTEST_GUILESS_MAIN(RoomListHeadingsProxyModelTest)

RoomListHeadingsProxyModelTest::RoomListHeadingsProxyModelTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

static QStandardItem *newItem(const char *text, RoomModel::Section section)
{
    auto item = new QStandardItem(QString::fromUtf8(text));
    item->setData(QVariant::fromValue(section), RoomModel::RoomSection);
    return item;
}

void RoomListHeadingsProxyModelTest::initTestCase()
{
    // 0 favorites, 2 teams, 1 discussion, 2 private messages
    int row = 0;
    mSourceModel.setItem(row++, 0, newItem("Team 1", RoomModel::Section::Teams));
    mSourceModel.setItem(row++, 0, newItem("Team 2", RoomModel::Section::Teams));
    mSourceModel.setItem(row++, 0, newItem("PM 1", RoomModel::Section::PrivateMessages));
    mSourceModel.setItem(row++, 0, newItem("Discuss 1", RoomModel::Section::Discussions));
    mSourceModel.setItem(row++, 0, newItem("Discuss 2", RoomModel::Section::Discussions));
    QCOMPARE(mSourceModel.rowCount(), row);
}

void RoomListHeadingsProxyModelTest::shouldBeEmptyByDefault()
{
    // GIVEN
    RoomListHeadingsProxyModel proxy;
    QStandardItemModel sourceModel;
    // WHEN
    proxy.setSourceModel(&sourceModel);
    // THEN
    QCOMPARE(proxy.rowCount(), 0); // this caught the missing std::fill on the std::array...
    QCOMPARE(proxy.sourceModel(), &sourceModel);
}

void RoomListHeadingsProxyModelTest::shouldReturnRowCount()
{
    // GIVEN
    RoomListHeadingsProxyModel proxy;
    proxy.setSourceModel(&mSourceModel);
    // WHEN
    const int count = proxy.rowCount();
    // THEN
    QCOMPARE(count, 5 + 3);
}

void RoomListHeadingsProxyModelTest::shouldMapProxyRows_data()
{
    QTest::addColumn<int>("proxyRow");
    QTest::addColumn<int>("expectedSourceRow");

    QTest::newRow("0") << 0 << -1;
    QTest::newRow("1") << 1 << 0;
    QTest::newRow("2") << 2 << 1;
    QTest::newRow("3") << 3 << -1;
    QTest::newRow("4") << 4 << 2;
    QTest::newRow("5") << 5 << -1;
    QTest::newRow("6") << 6 << 3;
    QTest::newRow("7") << 7 << 4;
}

void RoomListHeadingsProxyModelTest::shouldMapProxyRows()
{
    // GIVEN
    QFETCH(int, proxyRow);
    QFETCH(int, expectedSourceRow);
    RoomListHeadingsProxyModel proxy;
    proxy.setSourceModel(&mSourceModel);
    // WHEN
    const QModelIndex proxyIndex = proxy.index(proxyRow, 0);
    // THEN
    QCOMPARE(proxyIndex.row(), proxyRow);
    QCOMPARE(proxy.proxyRowToSourceRow(proxyRow), expectedSourceRow);
    if (expectedSourceRow == -1) {
        QVERIFY(!proxy.mapToSource(proxyIndex).isValid());
    } else {
        QCOMPARE(proxy.mapToSource(proxyIndex).row(), expectedSourceRow);
    }
}

void RoomListHeadingsProxyModelTest::shouldMapSourceRows_data()
{
    QTest::addColumn<int>("sourceRow");
    QTest::addColumn<int>("expectedProxyRow");

    QTest::newRow("0") << 0 << 1;
    QTest::newRow("1") << 1 << 2;
    QTest::newRow("2") << 2 << 4;
    QTest::newRow("3") << 3 << 6;
    QTest::newRow("4") << 4 << 7;
}

void RoomListHeadingsProxyModelTest::shouldMapSourceRows()
{
    // GIVEN
    QFETCH(int, sourceRow);
    QFETCH(int, expectedProxyRow);
    RoomListHeadingsProxyModel proxy;
    proxy.setSourceModel(&mSourceModel);
    // WHEN
    const QModelIndex sourceIndex = mSourceModel.index(sourceRow, 0);
    // THEN
    QCOMPARE(proxy.sourceRowToProxyRow(sourceRow), expectedProxyRow);
    QCOMPARE(proxy.mapFromSource(sourceIndex).row(), expectedProxyRow);
}

void RoomListHeadingsProxyModelTest::shouldDetermineProxyRowSection()
{
    // GIVEN
    RoomListHeadingsProxyModel proxy;
    proxy.setSourceModel(&mSourceModel);
    // WHEN/THEN
    const RoomModel::Section expected[] = {
        RoomModel::Section::Teams,
        RoomModel::Section::Teams,
        RoomModel::Section::Teams,
        RoomModel::Section::PrivateMessages,
        RoomModel::Section::PrivateMessages,
        RoomModel::Section::Discussions,
        RoomModel::Section::Discussions,
        RoomModel::Section::Discussions,
    };
    const int expectedRows = std::end(expected) - std::begin(expected); // need C++17 for std::size(expected)
    QCOMPARE(expectedRows, proxy.rowCount());
    for (size_t row = 0; row < expectedRows; ++row) {
        QCOMPARE(proxy.proxyRowSection(row), expected[row]);
    }
}

static QStringList initialExpectedList()
{
    return QStringList{QStringLiteral("Teams"),
                       QStringLiteral("Team 1"),
                       QStringLiteral("Team 2"),
                       QStringLiteral("Private Messages"),
                       QStringLiteral("PM 1"),
                       QStringLiteral("Discussions"),
                       QStringLiteral("Discuss 1"),
                       QStringLiteral("Discuss 2")};
}

static QStringList extractTexts(QAbstractItemModel *model)
{
    const int count = model->rowCount();
    QStringList texts;
    texts.reserve(count);
    for (int row = 0; row < count; ++row) {
        texts.append(model->index(row, 0).data().toString());
    }
    return texts;
}

static QStringList extractTexts(const std::vector<QPersistentModelIndex> &indexes)
{
    QStringList texts;
    texts.reserve(indexes.size());
    for (const QPersistentModelIndex &idx : indexes) {
        texts.append(idx.data().toString());
    }
    return texts;
}

static bool compareWithExpected(const QStringList &texts, const QStringList &expected)
{
    if (expected.count() != texts.count()) {
        qWarning() << "FAIL: expected" << expected.count() << "got" << texts.count() << '\n' << texts;
        return false;
    }
    for (int row = 0; row < expected.count(); ++row) {
        if (texts[row] != expected[row]) {
            qWarning() << "FAIL:" << row << texts[row] << expected[row];
            return false;
        }
    }
    return true;
}

void RoomListHeadingsProxyModelTest::shouldReturnData()
{
    // GIVEN
    RoomListHeadingsProxyModel proxy;
    proxy.setSourceModel(&mSourceModel);
    // WHEN/THEN
    QVERIFY(compareWithExpected(extractTexts(&proxy), initialExpectedList()));
}

void RoomListHeadingsProxyModelTest::shouldUpdateOnSectionUpdates()
{
    // GIVEN
    RoomListHeadingsProxyModel proxy;
    proxy.setSourceModel(&mSourceModel);

    // WHEN
    mSourceModel.item(0)->setData(QVariant::fromValue(RoomModel::Section::Favorites), RoomModel::RoomSection);

    // THEN
    const QStringList newExpected{QStringLiteral("Favorites"),
                                  QStringLiteral("Team 1"),
                                  QStringLiteral("Teams"),
                                  QStringLiteral("Team 2"),
                                  QStringLiteral("Private Messages"),
                                  QStringLiteral("PM 1"),
                                  QStringLiteral("Discussions"),
                                  QStringLiteral("Discuss 1"),
                                  QStringLiteral("Discuss 2")};
    QVERIFY(compareWithExpected(extractTexts(&proxy), newExpected));
}

void RoomListHeadingsProxyModelTest::shouldWorkOnTopOfQSFPM()
{
    // GIVEN
    RoomListHeadingsProxyModel proxy;
    RocketChatAccount account;
    account.setSortUnreadOnTop(true);
    account.setShowFavoriteRoom(true);
    RoomModel sampleModel(&account);
    int count = 0;
    std::vector<Room *> rooms;
    auto addRoom = [&](bool mainTeam, Room::RoomType roomType, const char *name) {
        auto room = new Room;
        room->setRoomId(QString::number(count));
        room->setName(QString::fromLatin1(name));
        room->setUnread(0);
        room->setParentRid(QStringLiteral("parentRId")); // not empty
        TeamInfo teamInfo;
        teamInfo.setMainTeam(mainTeam);
        room->setTeamInfo(teamInfo);
        room->setChannelType(roomType);
        room->setOpen(true);
        QVERIFY(sampleModel.addRoom(room));
        rooms.push_back(room);
        ++count;
    };
    addRoom(true, Room::RoomType::Channel, "Team 1");
    addRoom(true, Room::RoomType::Channel, "Team 2");
    addRoom(false, Room::RoomType::Direct, "PM 1");
    addRoom(false, Room::RoomType::Private, "Discuss 1");
    addRoom(false, Room::RoomType::Private, "Discuss 2");
    QCOMPARE(sampleModel.rowCount(), 5);
    RoomFilterProxyModel qsfpm;
    qsfpm.setSourceModel(&sampleModel);
    QCOMPARE(qsfpm.rowCount(), 5);

    // WHEN
    proxy.setSourceModel(&qsfpm);

    // THEN
    QVERIFY(compareWithExpected(extractTexts(&proxy), initialExpectedList()));

    // AND WHEN
    std::vector<QPersistentModelIndex> persistentIndexes;
    const int proxyCount = proxy.rowCount();
    for (int row = 0; row < proxyCount; ++row) {
        persistentIndexes.emplace_back(proxy.index(row, 0));
    }
    QVERIFY(compareWithExpected(extractTexts(persistentIndexes), initialExpectedList()));

    const QModelIndex discuss2Index = sampleModel.index(4, 0);
    QCOMPARE(discuss2Index.data().toString(), QStringLiteral("Discuss 2"));
    rooms[4]->setFavorite(true);
    Q_EMIT sampleModel.dataChanged(discuss2Index, discuss2Index);

    // THEN
    const QStringList newExpected{QStringLiteral("Favorites"),
                                  QStringLiteral("Discuss 2"),
                                  QStringLiteral("Teams"),
                                  QStringLiteral("Team 1"),
                                  QStringLiteral("Team 2"),
                                  QStringLiteral("Private Messages"),
                                  QStringLiteral("PM 1"),
                                  QStringLiteral("Discussions"),
                                  QStringLiteral("Discuss 1")};
    QVERIFY(compareWithExpected(extractTexts(&proxy), newExpected));

    const QStringList expectedPersistent{QString(),
                                         QStringLiteral("Team 1"),
                                         QStringLiteral("Team 2"),
                                         QString(),
                                         QStringLiteral("PM 1"),
                                         QString(),
                                         QStringLiteral("Discuss 1"),
                                         QStringLiteral("Discuss 2")};
    QVERIFY(compareWithExpected(extractTexts(persistentIndexes), expectedPersistent));

    // AND WHEN
    rooms[4]->setFavorite(false);
    Q_EMIT sampleModel.dataChanged(discuss2Index, discuss2Index);

    // THEN
    QVERIFY(compareWithExpected(extractTexts(&proxy), initialExpectedList()));
    QVERIFY(compareWithExpected(extractTexts(persistentIndexes), expectedPersistent));
}
