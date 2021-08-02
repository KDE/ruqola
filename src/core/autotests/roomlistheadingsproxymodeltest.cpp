#include "roomlistheadingsproxymodeltest.h"
#include "model/roomlistheadingsproxymodel.h"

#include <QStandardItemModel>
#include <QTest>

QTEST_GUILESS_MAIN(RoomListHeadingsProxyModelTest)

RoomListHeadingsProxyModelTest::RoomListHeadingsProxyModelTest(QObject *parent)
    : QObject(parent)
{
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
    if (expectedSourceRow == -1)
        QVERIFY(!proxy.mapToSource(proxyIndex).isValid());
    else
        QCOMPARE(proxy.mapToSource(proxyIndex).row(), expectedSourceRow);
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

void RoomListHeadingsProxyModelTest::shouldReturnData()
{
    // GIVEN
    RoomListHeadingsProxyModel proxy;
    proxy.setSourceModel(&mSourceModel);
    // WHEN/THEN
    const QStringList expected{QStringLiteral("Teams"),
                               QStringLiteral("Team 1"),
                               QStringLiteral("Team 2"),
                               QStringLiteral("Private Messages"),
                               QStringLiteral("PM 1"),
                               QStringLiteral("Discussions"),
                               QStringLiteral("Discuss 1"),
                               QStringLiteral("Discuss 2")};
    QCOMPARE(expected.count(), proxy.rowCount());
    for (int row = 0; row < expected.count(); ++row) {
        QCOMPARE(proxy.index(row, 0).data().toString(), expected[row]);
    }
}
