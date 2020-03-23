/*
   Copyright (c) 2020 Olivier de Gaalon <olivier.jg@gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "accountschannelsmodeltest.h"

#include "accountmanager.h"
#include "model/accountschannelsmodel.h"
#include "model/roomfilterproxymodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"

#include <QAbstractItemModelTester>
#include <QStandardPaths>
#include <QTest>

QTEST_MAIN(AccountsChannelsModelTest)

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
    QCOMPARE(model.data(model.index(1, 0)).toString(), QStringLiteral(""));
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
    QCOMPARE(model.rowCount(newAcctIndex), 0); // Room not yet open

    // FIXME: RoomModel should probably emit dataChanged to allow the sort/filter to update
    acct->roomModel()->findRoom(newRoomId)->setOpen(true);
    QEXPECT_FAIL("", "RoomModel missing dataChanged", Continue);
    QCOMPARE(model.rowCount(newAcctIndex), 1);
    // ... workaround for the above
    acct->roomFilterProxyModel()->invalidate();
    // ... and try again
    QCOMPARE(model.rowCount(newAcctIndex), 1);

    const auto newRoomIndex = model.index(0, 0, newAcctIndex);
    QVERIFY(!model.hasChildren(newRoomIndex));
    QCOMPARE(model.data(newRoomIndex).toString(), newRoomName);

    // TODO: RoomsModel currently has no API for removing rooms

    Ruqola::self()->accountManager()->removeAccount(newAcctName);
    QCOMPARE(model.rowCount(), 1); // Only the default account remains
}
