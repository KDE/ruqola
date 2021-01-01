/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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
