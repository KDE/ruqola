/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountmanagertest.h"

#include "accountmanager.h"
#include "localdatabase/localdatabaseutils.h"
#include "model/rocketchataccountfilterproxymodel.h"
#include "model/rocketchataccountmodel.h"
#include "rocketchataccount.h"
#include <QSettings>
#include <QStandardPaths>
#include <QTest>
#include <QUuid>
QTEST_GUILESS_MAIN(AccountManagerTest)

using namespace Qt::Literals::StringLiterals;
AccountManagerTest::AccountManagerTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void AccountManagerTest::shouldHaveDefaultValue()
{
    AccountManager w;
    QVERIFY(w.rocketChatAccountModel());
    QVERIFY(w.rocketChatAccountProxyModel());
    QCOMPARE(w.rocketChatAccountProxyModel()->sourceModel(), w.rocketChatAccountModel());

    QVERIFY(!w.account());
}

void AccountManagerTest::shouldAddAccount()
{
    AccountManager w;
    for (int i = 0; i < 10; ++i) {
        auto c = new RocketChatAccount();
        w.addAccount(c);
    }
    QCOMPARE(w.rocketChatAccountModel()->rowCount(), 10);
}

void AccountManagerTest::shouldReturnAccountFromAccountName()
{
    AccountManager w;
    QVERIFY(!w.accountFromName(u"DDDD"_s));

    auto c = new RocketChatAccount();
    QString accountName = u"bla"_s;
    c->setAccountName(accountName);
    w.addAccount(c);
    QVERIFY(w.accountFromName(accountName));

    QVERIFY(!w.accountFromName(u"DDDD"_s));

    accountName = u"bli"_s;
    c = new RocketChatAccount();
    c->setAccountName(accountName);
    w.addAccount(c);
    QVERIFY(w.accountFromName(accountName));
    w.removeAccount(u"bla"_s);
    w.removeAccount(u"bli"_s);
}

void AccountManagerTest::shouldClearCurrentAccountWhenRemovingLastAccount()
{
    AccountManager w;
    QSettings settings;
    settings.remove("currentAccount"_L1);

    const auto existingAccounts = w.accountsName();
    for (const auto &accountName : existingAccounts) {
        w.removeAccount(accountName);
    }

    const QString accountName = QUuid::createUuid().toString(QUuid::WithoutBraces);
    auto account = new RocketChatAccount();
    account->setAccountName(accountName);
    w.addAccount(account);

    QCOMPARE(w.currentAccount(), accountName);
    w.removeAccount(accountName);
    QVERIFY(!w.accountsName().contains(accountName));
    QVERIFY(w.currentAccount() != accountName);
    QCOMPARE(settings.value("currentAccount"_L1).toString(), w.currentAccount());
}

void AccountManagerTest::shouldCheckPathsToRemoved()
{
    QFETCH(AccountManager::MigrateDatabaseTypes, types);
    QFETCH(QStringList, paths);

    QCOMPARE(AccountManager::databasePathsToRemoved(types), paths);
}

void AccountManagerTest::shouldCheckPathsToRemoved_data()
{
    QTest::addColumn<AccountManager::MigrateDatabaseTypes>("types");
    QTest::addColumn<QStringList>("paths");
    {
        const AccountManager::MigrateDatabaseTypes t = AccountManager::MigrateDatabaseType::None;
        QTest::addRow("None") << t << QStringList();
    }
    {
        const AccountManager::MigrateDatabaseTypes t = AccountManager::MigrateDatabaseType::DatabaseRooms;
        QTest::addRow("DatabaseRooms") << t << QStringList({LocalDatabaseUtils::localRoomsDatabasePath()});
    }
    {
        AccountManager::MigrateDatabaseTypes t = AccountManager::MigrateDatabaseType::DatabaseRooms;
        t |= AccountManager::MigrateDatabaseType::DatabaseMessages;
        QTest::addRow("DatabaseRooms+DatabaseMessages")
            << t << QStringList({LocalDatabaseUtils::localRoomsDatabasePath(), LocalDatabaseUtils::localMessagesDatabasePath()});
    }
    {
        AccountManager::MigrateDatabaseTypes t = AccountManager::MigrateDatabaseType::All;
        QStringList lst;
        lst.append(LocalDatabaseUtils::localRoomsDatabasePath());
        lst.append(LocalDatabaseUtils::localAccountsDatabasePath());
        lst.append(LocalDatabaseUtils::localE2EDatabasePath());
        lst.append(LocalDatabaseUtils::localRoomPendingTypedInfoDatabasePath());
        lst.append(LocalDatabaseUtils::localRoomSubscriptionsDatabasePath());
        lst.append(LocalDatabaseUtils::localMessagesDatabasePath());
        lst.append(LocalDatabaseUtils::localMessageLoggerPath());
        lst.append(LocalDatabaseUtils::localGlobalDatabasePath());
        QTest::addRow("All") << t << lst;
    }
    {
        AccountManager::MigrateDatabaseTypes t = AccountManager::MigrateDatabaseType::DatabaseWithoutLogger;
        QStringList lst;
        lst.append(LocalDatabaseUtils::localRoomsDatabasePath());
        lst.append(LocalDatabaseUtils::localAccountsDatabasePath());
        lst.append(LocalDatabaseUtils::localE2EDatabasePath());
        lst.append(LocalDatabaseUtils::localRoomPendingTypedInfoDatabasePath());
        lst.append(LocalDatabaseUtils::localRoomSubscriptionsDatabasePath());
        lst.append(LocalDatabaseUtils::localMessagesDatabasePath());
        lst.append(LocalDatabaseUtils::localGlobalDatabasePath());
        QTest::addRow("DatabaseWithoutLogger") << t << lst;
    }
    {
        AccountManager::MigrateDatabaseTypes t = AccountManager::MigrateDatabaseType::DatabaseLogger;
        QStringList lst;
        lst.append(LocalDatabaseUtils::localMessageLoggerPath());
        QTest::addRow("Logger") << t << lst;
    }
}

#include "moc_accountmanagertest.cpp"
