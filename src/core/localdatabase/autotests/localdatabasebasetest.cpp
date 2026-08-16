/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabasebasetest.h"

#include "localdatabase/localdatabasebase.h"
#include <QSqlDatabase>
#include <QTemporaryDir>
#include <QTest>

using namespace Qt::Literals::StringLiterals;
class TestLocalDatabaseBase : public LocalDatabaseBase
{
public:
    explicit TestLocalDatabaseBase(const QString &basePath, LocalDatabaseBase::DatabaseType type)
        : LocalDatabaseBase(basePath, type)
    {
    }
    ~TestLocalDatabaseBase() override = default;
    [[nodiscard]] QString currentDatabaseName(const QString &name) const
    {
        return databaseName(name);
    }
    [[nodiscard]] QString schemaDataBase() const override
    {
        return u"CREATE TABLE TESTTABLE (identifier TEXT PRIMARY KEY NOT NULL)"_s;
    }
    [[nodiscard]] bool createDataBase(const QString &accountName)
    {
        QSqlDatabase db;
        return initializeDataBase(accountName, db);
    }
    [[nodiscard]] bool createDataBase(const QString &accountName, const QByteArray &roomId)
    {
        QSqlDatabase db;
        return initializeDataBase(accountName, roomId, db);
    }
};

QTEST_MAIN(LocalDatabaseBaseTest)
LocalDatabaseBaseTest::LocalDatabaseBaseTest(QObject *parent)
    : QObject{parent}
{
}

void LocalDatabaseBaseTest::shouldHaveDefaultValues()
{
    {
        TestLocalDatabaseBase w(u"foo/bla/"_s, LocalDatabaseBase::DatabaseType::Accounts);
        QCOMPARE(w.currentDatabaseName(u"kde"_s), u"accounts-kde"_s);
    }
    {
        TestLocalDatabaseBase w(u"foo/bla/"_s, LocalDatabaseBase::DatabaseType::Rooms);
        QCOMPARE(w.currentDatabaseName(u"kde"_s), u"rooms-kde"_s);
    }
    {
        TestLocalDatabaseBase w(u"foo/bla/"_s, LocalDatabaseBase::DatabaseType::Messages);
        QCOMPARE(w.currentDatabaseName(u"kde"_s), u"messages-kde"_s);
    }
    {
        TestLocalDatabaseBase w(u"foo/bla/"_s, LocalDatabaseBase::DatabaseType::Logger);
        QCOMPARE(w.currentDatabaseName(u"kde"_s), u"kde"_s);
    }
    {
        TestLocalDatabaseBase w(u"foo/bla/"_s, LocalDatabaseBase::DatabaseType::Global);
        QCOMPARE(w.currentDatabaseName(u"kde"_s), u"global-kde"_s);
    }
    {
        TestLocalDatabaseBase w(u"foo/bla/"_s, LocalDatabaseBase::DatabaseType::E2E);
        QCOMPARE(w.currentDatabaseName(u"kde"_s), u"e2e-kde"_s);
    }
    {
        TestLocalDatabaseBase w(u"foo/bla/"_s, LocalDatabaseBase::DatabaseType::PendingTypedInfo);
        QCOMPARE(w.currentDatabaseName(u"kde"_s), u"pendingtypedinfo-kde"_s);
    }
    {
        TestLocalDatabaseBase w(u"foo/bla/"_s, LocalDatabaseBase::DatabaseType::RoomSubscriptions);
        QCOMPARE(w.currentDatabaseName(u"kde"_s), u"roomsubscriptions-kde"_s);
    }
    {
        TestLocalDatabaseBase w(u"foo/bla/"_s, LocalDatabaseBase::DatabaseType::E2ERooms);
        QCOMPARE(w.currentDatabaseName(u"kde"_s), u"e2e-rooms-kde"_s);
    }
}

void LocalDatabaseBaseTest::shouldRemoveDataBaseConnections()
{
    QTemporaryDir tmpDir;
    QVERIFY(tmpDir.isValid());
    const QString basePath = tmpDir.path() + u'/';

    TestLocalDatabaseBase rooms(basePath + u"rooms/"_s, LocalDatabaseBase::DatabaseType::Rooms);
    TestLocalDatabaseBase messages(basePath + u"messages/"_s, LocalDatabaseBase::DatabaseType::Messages);
    QVERIFY(rooms.createDataBase(u"kde"_s));
    QVERIFY(rooms.createDataBase(u"kde2"_s));
    QVERIFY(messages.createDataBase(u"kde"_s, "roomId1"));

    QVERIFY(QSqlDatabase::contains(u"rooms-kde"_s));
    QVERIFY(QSqlDatabase::contains(u"rooms-kde2"_s));
    QVERIFY(QSqlDatabase::contains(u"messages-kde-roomId1"_s));

    LocalDatabaseBase::removeDataBaseConnections(u"kde"_s);

    QVERIFY(!QSqlDatabase::contains(u"rooms-kde"_s));
    QVERIFY(!QSqlDatabase::contains(u"messages-kde-roomId1"_s));
    // An account whose name merely starts with the removed one must be left alone
    QVERIFY(QSqlDatabase::contains(u"rooms-kde2"_s));

    // The connection is gone but the file is still there: reopening must find the existing schema
    QVERIFY(rooms.createDataBase(u"kde"_s));
    QVERIFY(QSqlDatabase::contains(u"rooms-kde"_s));

    LocalDatabaseBase::removeDataBaseConnections(u"kde"_s);
    LocalDatabaseBase::removeDataBaseConnections(u"kde2"_s);
}

#include "moc_localdatabasebasetest.cpp"
