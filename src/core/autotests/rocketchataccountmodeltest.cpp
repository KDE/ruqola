/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchataccountmodeltest.h"
using namespace Qt::Literals::StringLiterals;

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
    QList<RocketChatAccount *> accounts;
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
    QList<RocketChatAccount *> accounts;
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
    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();
    modelResetSpy.clear();
    modelABTReset.clear();

    w.clear();
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(modelResetSpy.count(), 1);
    QCOMPARE(modelABTReset.count(), 1);
}

void RocketChatAccountModelTest::shouldRemoveAccount()
{
    RocketChatAccountModel w;
    QList<RocketChatAccount *> accounts;
    for (int i = 0; i < 10; ++i) {
        auto f = new RocketChatAccount();
        f->setAccountName(u"foo%1"_s.arg(i));
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

    w.removeAccount(u"foo2"_s);

    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), u"2,2"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), u"2,2"_s);
    QCOMPARE(modelResetSpy.count(), 0);
    QCOMPARE(modelABTReset.count(), 0);

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();
    modelResetSpy.clear();
    modelABTReset.clear();

    w.removeAccount(u"notexist"_s);

    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(modelResetSpy.count(), 0);
    QCOMPARE(modelABTReset.count(), 0);
}

#include "moc_rocketchataccountmodeltest.cpp"
