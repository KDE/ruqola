/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "threadsmodeltest.h"
#include "model/threadsmodel.h"
#include "threads.h"
#include "test_model_helpers.h"
#include <QTest>
QTEST_GUILESS_MAIN(ThreadsModelTest)

ThreadsModelTest::ThreadsModelTest(QObject *parent)
    : QObject(parent)
{
}

void ThreadsModelTest::shouldHaveDefaultValues()
{
    ThreadsModel model;
    QHash<int, QByteArray> roles;
    roles[ThreadsModel::ThreadMessageId] = QByteArrayLiteral("threadmessageid");
    roles[ThreadsModel::Description] = QByteArrayLiteral("description");
    roles[ThreadsModel::NumberOfMessages] = QByteArrayLiteral("numberofmessages");
    roles[ThreadsModel::LastMessage] = QByteArrayLiteral("lastmessage");
    roles[ThreadsModel::TimeStamp] = QByteArrayLiteral("timestamp");
    roles[ThreadsModel::UserName] = QByteArrayLiteral("username");
    roles[ThreadsModel::AliasName] = QByteArrayLiteral("aliasname");
    roles[ThreadsModel::Avatar] = QByteArrayLiteral("avatar");
    roles[ThreadsModel::UserId] = QByteArrayLiteral("userid");

    QCOMPARE(model.roleNames(), roles);
}

void ThreadsModelTest::shouldAssignValues()
{
    ThreadsModel w;
    QSignalSpy rowInsertedSpy(&w, &ThreadsModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &ThreadsModel::rowsAboutToBeInserted);
    QSignalSpy rowRemovedSpy(&w, &ThreadsModel::rowsRemoved);
    QSignalSpy rowABTRemoved(&w, &ThreadsModel::rowsAboutToBeRemoved);

    Threads ThreadList;
    for (int i = 0; i < 10; ++i) {
        Thread c;
        c.setMessageId(QStringLiteral("roomid%1").arg(i));
        ThreadList.append(c);
    }
    w.setThreads(ThreadList);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,9"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,9"));

    //add Empty list
    ThreadList.clear();
    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    w.setThreads(ThreadList);

    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QStringLiteral("0,9"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QStringLiteral("0,9"));

    //Add same element
    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    w.setThreads(ThreadList);

    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QString());
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QString());
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QString());
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QString());

    //Test add same number of element.
    ThreadList.clear();
    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    for (int i = 0; i < 5; ++i) {
        Thread c;
        c.setMessageId(QStringLiteral("roomid%1").arg(i));
        ThreadList.append(c);
    }
    w.setThreads(ThreadList);
    QCOMPARE(w.rowCount(), 5);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,4"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,4"));

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();
    w.setThreads(ThreadList);

    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QStringLiteral("0,4"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QStringLiteral("0,4"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,4"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,4"));
}
