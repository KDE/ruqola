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
    QCOMPARE(proxy.rowCount({}), proxy.sectionCount);
    for (uint section = 0; section < proxy.sectionCount; ++section) {
        QCOMPARE(proxy.rowCount(proxy.index(section, 0, {})), 0);
    }
    QCOMPARE(proxy.sourceModel(), &sourceModel);
}

void RoomListHeadingsProxyModelTest::shouldReturnRowCount()
{
    // GIVEN
    RoomListHeadingsProxyModel proxy;
    // WHEN
    proxy.setSourceModel(&mSourceModel);
    // THEN
    const auto expectedCount = [](RoomModel::Section section) {
        switch (section) {
        case RoomModel::Section::Unread:
        case RoomModel::Section::Favorites:
        case RoomModel::Section::Rooms:
        case RoomModel::Section::Unknown:
            return 0;
        case RoomModel::Section::Teams:
        case RoomModel::Section::Discussions:
            return 2;
        case RoomModel::Section::PrivateMessages:
            return 1;
        case RoomModel::Section::NSections:
            Q_UNREACHABLE();
        }
        Q_UNREACHABLE();
    };

    for (uint section = 0; section < proxy.sectionCount; ++section) {
        QCOMPARE(proxy.rowCount(proxy.index(section, 0, {})), expectedCount(RoomModel::Section(section)));
    }
}

void RoomListHeadingsProxyModelTest::shouldMapProxyRows_data()
{
    QTest::addColumn<RoomModel::Section>("section");
    QTest::addColumn<int>("proxyRow");
    QTest::addColumn<int>("expectedSourceRow");

    QTest::newRow("0") << RoomModel::Section::Teams << 0 << 0;
    QTest::newRow("1") << RoomModel::Section::Teams << 1 << 1;
    QTest::newRow("2") << RoomModel::Section::PrivateMessages << 0 << 2;
    QTest::newRow("3") << RoomModel::Section::Discussions << 0 << 3;
    QTest::newRow("4") << RoomModel::Section::Discussions << 1 << 4;
}

void RoomListHeadingsProxyModelTest::shouldMapProxyRows()
{
    // GIVEN
    QFETCH(RoomModel::Section, section);
    QFETCH(int, proxyRow);
    QFETCH(int, expectedSourceRow);
    RoomListHeadingsProxyModel proxy;
    proxy.setSourceModel(&mSourceModel);
    // WHEN
    const QModelIndex proxyIndex = proxy.index(proxyRow, 0, proxy.index(int(section), 0, {}));
    // THEN
    QCOMPARE(proxy.mapToSource(proxyIndex).row(), expectedSourceRow);
}

void RoomListHeadingsProxyModelTest::shouldMapSourceRows_data()
{
    QTest::addColumn<int>("sourceRow");
    QTest::addColumn<RoomModel::Section>("expectedSection");
    QTest::addColumn<int>("expectedProxyRow");

    QTest::newRow("0") << 0 << RoomModel::Section::Teams << 0;
    QTest::newRow("1") << 1 << RoomModel::Section::Teams << 1;
    QTest::newRow("2") << 2 << RoomModel::Section::PrivateMessages << 0;
    QTest::newRow("3") << 3 << RoomModel::Section::Discussions << 0;
    QTest::newRow("4") << 4 << RoomModel::Section::Discussions << 1;
}

void RoomListHeadingsProxyModelTest::shouldMapSourceRows()
{
    // GIVEN
    QFETCH(int, sourceRow);
    QFETCH(RoomModel::Section, expectedSection);
    QFETCH(int, expectedProxyRow);
    RoomListHeadingsProxyModel proxy;
    proxy.setSourceModel(&mSourceModel);
    // WHEN
    const QModelIndex sourceIndex = mSourceModel.index(sourceRow, 0);
    // THEN
    const QModelIndex proxyIndex = proxy.mapFromSource(sourceIndex);
    QCOMPARE(proxyIndex.row(), expectedProxyRow);
    QCOMPARE(proxyIndex.parent().row(), int(expectedSection));
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
    for (int sectionId = 0; sectionId < count; ++sectionId) {
        const auto section = model->index(sectionId, 0, {});
        const auto sectionSize = model->rowCount(section);

        if (sectionSize == 0)
            continue;

        texts.append(section.data().toString());

        for (int row = 0; row < sectionSize; ++row) {
            texts.append(model->index(row, 0, section).data().toString());
        }
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
