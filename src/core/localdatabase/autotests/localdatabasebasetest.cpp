/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabasebasetest.h"
#include "localdatabase/localdatabasebase.h"
#include <QTest>

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
};

QTEST_MAIN(LocalDatabaseBaseTest)
LocalDatabaseBaseTest::LocalDatabaseBaseTest(QObject *parent)
    : QObject{parent}
{
}

void LocalDatabaseBaseTest::shouldHaveDefaultValues()
{
    {
        TestLocalDatabaseBase w(QStringLiteral("foo/bla/"), LocalDatabaseBase::DatabaseType::Account);
        QCOMPARE(w.currentDatabaseName(QStringLiteral("kde")), QStringLiteral("accounts-kde"));
    }
    {
        TestLocalDatabaseBase w(QStringLiteral("foo/bla/"), LocalDatabaseBase::DatabaseType::Rooms);
        QCOMPARE(w.currentDatabaseName(QStringLiteral("kde")), QStringLiteral("rooms-kde"));
    }
    {
        TestLocalDatabaseBase w(QStringLiteral("foo/bla/"), LocalDatabaseBase::DatabaseType::Message);
        QCOMPARE(w.currentDatabaseName(QStringLiteral("kde")), QStringLiteral("messages-kde"));
    }
    {
        TestLocalDatabaseBase w(QStringLiteral("foo/bla/"), LocalDatabaseBase::DatabaseType::Logger);
        QCOMPARE(w.currentDatabaseName(QStringLiteral("kde")), QStringLiteral("kde"));
    }
    {
        TestLocalDatabaseBase w(QStringLiteral("foo/bla/"), LocalDatabaseBase::DatabaseType::Global);
        QCOMPARE(w.currentDatabaseName(QStringLiteral("kde")), QStringLiteral("global-kde"));
    }
}
#include "moc_localdatabasebasetest.cpp"
