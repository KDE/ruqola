/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "usersforroommodeltest.h"
#include "test_model_helpers.h"
#include "model/usersforroommodel.h"
#include <QSignalSpy>
#include <QTest>
QTEST_GUILESS_MAIN(UsersForRoomModelTest)

UsersForRoomModelTest::UsersForRoomModelTest(QObject *parent)
    : QObject(parent)
{
}

void UsersForRoomModelTest::shouldHaveDefaultValue()
{
    UsersForRoomModel w;
    QSignalSpy rowInsertedSpy(&w, &UsersForRoomModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &UsersForRoomModel::rowsAboutToBeInserted);
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);

    QHash<int, QByteArray> roles;
    roles[UsersForRoomModel::UserName] = QByteArrayLiteral("username");
    roles[UsersForRoomModel::Name] = QByteArrayLiteral("name");
    roles[UsersForRoomModel::IconStatus] = QByteArrayLiteral("iconstatus");
    roles[UsersForRoomModel::DisplayName] = QByteArrayLiteral("displayname");
    QCOMPARE(w.roleNames(), roles);
}

void UsersForRoomModelTest::shouldAddValues()
{
    UsersForRoomModel w;
    QVector<User> users;
    QSignalSpy rowInsertedSpy(&w, &UsersForRoomModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &UsersForRoomModel::rowsAboutToBeInserted);
    for (int i = 0; i < 10; i++) {
        User user;
        user.setName(QStringLiteral("name%1").arg(i));
        user.setStatus(QStringLiteral("status%1").arg(i));
        user.setUserId(QStringLiteral("userId%1").arg(i));
        user.setUserName(QStringLiteral("username%1").arg(i));
        users.append(user);
    }
    w.setUsers(users);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,9"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,9"));
    rowABTInserted.clear();
    rowInsertedSpy.clear();

    users.clear();
    for (int i = 0; i < 3; ++i) {
        User user;
        user.setName(QStringLiteral("name%1").arg(i));
        user.setStatus(QStringLiteral("status%1").arg(i));
        user.setUserId(QStringLiteral("userId%1").arg(i));
        user.setUserName(QStringLiteral("username%1").arg(i));
        users.append(user);
    }
    w.setUsers(users);
    QCOMPARE(w.rowCount(), 3);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,2"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,2"));

    rowABTInserted.clear();
    rowInsertedSpy.clear();

    users.clear();
    w.setUsers(users);
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QString());
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QString());
}

void UsersForRoomModelTest::shouldVerifyData()
{
    UsersForRoomModel w;
    QVector<User> users;
    for (int i = 0; i < 10; i++) {
        User user;
        user.setName(QStringLiteral("name%1").arg(i));
        user.setStatus(QStringLiteral("status%1").arg(i));
        user.setUserId(QStringLiteral("userId%1").arg(i));
        user.setUserName(QStringLiteral("username%1").arg(i));
        users.append(user);
    }
    w.setUsers(users);
    QCOMPARE(w.rowCount(), 10);

    for (int i = 0; i < 10; ++i) {
        QCOMPARE(w.data(w.index(i), UsersForRoomModel::Name).toString(), QStringLiteral("name%1").arg(i));
        //TODO fix me.QCOMPARE(w.data(w.index(i), UsersForRoomModel::IconStatus).toString(), QStringLiteral("description%1").arg(i));
        QCOMPARE(w.data(w.index(i), UsersForRoomModel::UserId).toString(), QStringLiteral("userId%1").arg(i));
        QCOMPARE(w.data(w.index(i), UsersForRoomModel::UserName).toString(), QStringLiteral("username%1").arg(i));
    }
}
