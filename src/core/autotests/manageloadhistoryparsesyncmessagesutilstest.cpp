/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "manageloadhistoryparsesyncmessagesutilstest.h"
using namespace Qt::Literals::StringLiterals;

#include "manageloadhistoryparsesyncmessagesutils.h"
#include "ruqola_autotest_helper.h"

QTEST_MAIN(ManageLoadHistoryParseSyncMessagesUtilsTest)
ManageLoadHistoryParseSyncMessagesUtilsTest::ManageLoadHistoryParseSyncMessagesUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void ManageLoadHistoryParseSyncMessagesUtilsTest::shouldHaveDefaultValues()
{
    ManageLoadHistoryParseSyncMessagesUtils w(nullptr);
    QVERIFY(w.deletedMessages().isEmpty());
    QVERIFY(w.updatesMessages().isEmpty());
}

void ManageLoadHistoryParseSyncMessagesUtilsTest::shouldParseSyncMessages_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("deleted");
    QTest::addColumn<int>("update");

    QTest::addRow("empty") << u"empty.json"_s << 0 << 0;
    QTest::addRow("test1") << u"test1.json"_s << 1 << 0;
    QTest::addRow("test2") << u"test2.json"_s << 1 << 2;
}

void ManageLoadHistoryParseSyncMessagesUtilsTest::shouldParseSyncMessages()
{
    QFETCH(QString, name);
    QFETCH(int, deleted);
    QFETCH(int, update);

    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/syncmessages/%1"_L1.arg(name);
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    ManageLoadHistoryParseSyncMessagesUtils w(nullptr);
    w.parse(obj);
    QCOMPARE(w.deletedMessages().count(), deleted);
    QCOMPARE(w.updatesMessages().count(), update);
}

#include "moc_manageloadhistoryparsesyncmessagesutilstest.cpp"
