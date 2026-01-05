/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabasebasetest.h"

#include "localdatabase/localdatabasebase.h"
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
}
#include "moc_localdatabasebasetest.cpp"
