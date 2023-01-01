/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersmodeltest.h"
#include "model/usersmodel.h"
#include <QSignalSpy>
#include <QTest>

QTEST_GUILESS_MAIN(UsersModelTest)

UsersModelTest::UsersModelTest(QObject *parent)
    : QObject(parent)
{
}

void UsersModelTest::shouldHaveDefaultValue()
{
    UsersModel w;
    QSignalSpy rowInsertedSpy(&w, &UsersModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &UsersModel::rowsAboutToBeInserted);
    QSignalSpy rowRemovedSpy(&w, &UsersModel::rowsRemoved);
    QSignalSpy rowABTRemoved(&w, &UsersModel::rowsAboutToBeRemoved);
    QSignalSpy userStatusChangedSpy(&w, &UsersModel::userStatusChanged);
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(userStatusChangedSpy.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
}

void UsersModelTest::shouldRemoveUser()
{
    UsersModel w;
    QSignalSpy rowInsertedSpy(&w, &UsersModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &UsersModel::rowsAboutToBeInserted);
    QSignalSpy rowRemovedSpy(&w, &UsersModel::rowsRemoved);
    QSignalSpy rowABTRemoved(&w, &UsersModel::rowsAboutToBeRemoved);
    QSignalSpy userStatusChangedSpy(&w, &UsersModel::userStatusChanged);
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(userStatusChangedSpy.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
}
