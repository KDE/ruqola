/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "rocketchataccountmodeltest.h"
#include "model/rocketchataccountmodel.h"
#include "rocketchataccount.h"
#include "test_model_helpers.h"
#include <QTest>
#include <QSignalSpy>
#include <QStandardPaths>
QTEST_MAIN(RocketChatAccountModelTest)

RocketChatAccountModelTest::RocketChatAccountModelTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void RocketChatAccountModelTest::shouldHaveDefaultValue()
{
    RocketChatAccountModel w;

    QHash<int, QByteArray> roles;
    roles[RocketChatAccountModel::Name] = QByteArrayLiteral("name");
    roles[RocketChatAccountModel::SiteUrl] = QByteArrayLiteral("siteurl");
    roles[RocketChatAccountModel::UserName] = QByteArrayLiteral("username");

    QCOMPARE(w.roleNames(), roles);
}

void RocketChatAccountModelTest::shouldAddAccountValue()
{
    RocketChatAccountModel w;
    QVector<RocketChatAccount *> accounts;
    for (int i = 0; i < 10; ++i) {
        RocketChatAccount *f = new RocketChatAccount();
        accounts.append(f);
    }
    QSignalSpy rowInsertedSpy(&w, &RocketChatAccountModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &RocketChatAccountModel::rowsAboutToBeInserted);
    QSignalSpy rowRemovedSpy(&w, &RocketChatAccountModel::rowsRemoved);
    QSignalSpy rowABTRemoved(&w, &RocketChatAccountModel::rowsAboutToBeRemoved);

    w.setAccounts(accounts);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    qDeleteAll(accounts);
    accounts.clear();
    for (int i = 0; i < 3; ++i) {
        RocketChatAccount *f = new RocketChatAccount();
        accounts.append(f);
    }

    w.setAccounts(accounts);

    QCOMPARE(w.rowCount(), 3);

    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
}

void RocketChatAccountModelTest::shouldClearAccounts()
{
    RocketChatAccountModel w;
    QVector<RocketChatAccount *> accounts;
    for (int i = 0; i < 10; ++i) {
        RocketChatAccount *f = new RocketChatAccount();
        accounts.append(f);
    }
    QSignalSpy rowInsertedSpy(&w, &RocketChatAccountModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &RocketChatAccountModel::rowsAboutToBeInserted);
    QSignalSpy rowRemovedSpy(&w, &RocketChatAccountModel::rowsRemoved);
    QSignalSpy rowABTRemoved(&w, &RocketChatAccountModel::rowsAboutToBeRemoved);

    w.setAccounts(accounts);
    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    w.clear();
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QStringLiteral("0,9"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QStringLiteral("0,9"));
}

void RocketChatAccountModelTest::shouldRemoveAccount()
{
    RocketChatAccountModel w;
    QVector<RocketChatAccount *> accounts;
    for (int i = 0; i < 10; ++i) {
        RocketChatAccount *f = new RocketChatAccount();
        f->setAccountName(QStringLiteral("foo%1").arg(i));
        accounts.append(f);
    }
    QSignalSpy rowInsertedSpy(&w, &RocketChatAccountModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &RocketChatAccountModel::rowsAboutToBeInserted);
    QSignalSpy rowRemovedSpy(&w, &RocketChatAccountModel::rowsRemoved);
    QSignalSpy rowABTRemoved(&w, &RocketChatAccountModel::rowsAboutToBeRemoved);

    w.setAccounts(accounts);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    w.removeAccount(QStringLiteral("foo2"));

    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QStringLiteral("2,2"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QStringLiteral("2,2"));

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    w.removeAccount(QStringLiteral("notexist"));

    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
}
