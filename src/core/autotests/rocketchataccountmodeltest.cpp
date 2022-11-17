/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchataccountmodeltest.h"
#include "model/rocketchataccountmodel.h"
#include "rocketchataccount.h"
#include "test_model_helpers.h"
#include <QSignalSpy>
#include <QStandardPaths>
#include <QTest>
QTEST_GUILESS_MAIN(RocketChatAccountModelTest)

RocketChatAccountModelTest::RocketChatAccountModelTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void RocketChatAccountModelTest::shouldHaveDefaultValue()
{
    RocketChatAccountModel w;
    // TODO
}

void RocketChatAccountModelTest::shouldAddAccountValue()
{
    RocketChatAccountModel w;
    QVector<RocketChatAccount *> accounts;
    accounts.reserve(10);
    for (int i = 0; i < 10; ++i) {
        auto f = new RocketChatAccount();
        accounts.append(f);
    }
    QSignalSpy rowInsertedSpy(&w, &RocketChatAccountModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &RocketChatAccountModel::rowsAboutToBeInserted);
    QSignalSpy rowRemovedSpy(&w, &RocketChatAccountModel::rowsRemoved);
    QSignalSpy rowABTRemoved(&w, &RocketChatAccountModel::rowsAboutToBeRemoved);
    QSignalSpy modelResetSpy(&w, &RocketChatAccountModel::modelReset);
    QSignalSpy modelABTReset(&w, &RocketChatAccountModel::modelAboutToBeReset);

    w.setAccounts(accounts);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(modelResetSpy.count(), 1);
    QCOMPARE(modelABTReset.count(), 1);

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();
    modelResetSpy.clear();
    modelABTReset.clear();

    qDeleteAll(accounts);
    accounts.clear();
    accounts.reserve(3);
    for (int i = 0; i < 3; ++i) {
        auto f = new RocketChatAccount();
        accounts.append(f);
    }

    w.setAccounts(accounts);

    QCOMPARE(w.rowCount(), 3);

    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(modelResetSpy.count(), 1);
    QCOMPARE(modelABTReset.count(), 1);
}

void RocketChatAccountModelTest::shouldClearAccounts()
{
    RocketChatAccountModel w;
    QVector<RocketChatAccount *> accounts;
    accounts.reserve(10);
    for (int i = 0; i < 10; ++i) {
        auto f = new RocketChatAccount();
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
        auto f = new RocketChatAccount();
        f->setAccountName(QStringLiteral("foo%1").arg(i));
        accounts.append(f);
    }
    QSignalSpy rowInsertedSpy(&w, &RocketChatAccountModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &RocketChatAccountModel::rowsAboutToBeInserted);
    QSignalSpy rowRemovedSpy(&w, &RocketChatAccountModel::rowsRemoved);
    QSignalSpy rowABTRemoved(&w, &RocketChatAccountModel::rowsAboutToBeRemoved);
    QSignalSpy modelResetSpy(&w, &RocketChatAccountModel::modelReset);
    QSignalSpy modelABTReset(&w, &RocketChatAccountModel::modelAboutToBeReset);

    w.setAccounts(accounts);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(modelResetSpy.count(), 1);
    QCOMPARE(modelABTReset.count(), 1);

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();
    modelResetSpy.clear();
    modelABTReset.clear();

    w.removeAccount(QStringLiteral("foo2"));

    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QStringLiteral("2,2"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QStringLiteral("2,2"));
    QCOMPARE(modelResetSpy.count(), 0);
    QCOMPARE(modelABTReset.count(), 0);

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();
    modelResetSpy.clear();
    modelABTReset.clear();

    w.removeAccount(QStringLiteral("notexist"));

    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(modelResetSpy.count(), 0);
    QCOMPARE(modelABTReset.count(), 0);
}
