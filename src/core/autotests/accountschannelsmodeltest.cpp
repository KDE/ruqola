/*
   SPDX-FileCopyrightText: 2020 Olivier de Gaalon <olivier.jg@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountschannelsmodeltest.h"

#include "accountmanager.h"
#include "model/accountschannelsmodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"

#include <QAbstractItemModelTester>
#include <QStandardPaths>
#include <QTest>

QTEST_GUILESS_MAIN(AccountsChannelsModelTest)

AccountsChannelsModelTest::AccountsChannelsModelTest(QObject *parent)
    : QObject(parent)
{
}

void AccountsChannelsModelTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
}

void AccountsChannelsModelTest::accountsAndChannels()
{
    AccountsChannelsModel model;
    QAbstractItemModelTester tester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);

    QCOMPARE(model.rowCount(), 1); // Ruqola creates one account by default
    QCOMPARE(model.data(model.index(1, 0)).toString(), QString());
    QCOMPARE(model.rowCount(model.index(1, 0)), 0);

    const auto newAcctName = QStringLiteral("Test Account");
    const auto acct = new RocketChatAccount;
    Ruqola::self()->accountManager()->addAccount(acct);
    const auto newAcctIndex = model.index(1, 0);
    QCOMPARE(model.rowCount(), 2);
    QVERIFY(!model.hasChildren(newAcctIndex));
    QCOMPARE(model.data(newAcctIndex).toString(), QString());
    acct->setAccountName(newAcctName);
    QCOMPARE(model.data(newAcctIndex).toString(), newAcctName);

    Ruqola::self()->setCurrentAccount(newAcctName);

    const auto newRoomId = QStringLiteral("RoomId");
    const auto newRoomName = QStringLiteral("Room Name");
    acct->roomModel()->addRoom(newRoomId, newRoomName);
    QCOMPARE(model.rowCount(newAcctIndex), 1);

    const auto newRoomIndex = model.index(0, 0, newAcctIndex);
    QVERIFY(!model.hasChildren(newRoomIndex));
    QCOMPARE(model.data(newRoomIndex).toString(), newRoomName);

    // TODO: RoomsModel currently has no API for removing rooms

    Ruqola::self()->accountManager()->removeAccount(newAcctName);
    QCOMPARE(model.rowCount(), 1); // Only the default account remains
}
