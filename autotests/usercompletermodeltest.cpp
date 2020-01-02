/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "usercompletermodeltest.h"
#include "model/usercompletermodel.h"
#include "test_model_helpers.h"
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(UserCompleterModelTest)

UserCompleterModelTest::UserCompleterModelTest(QObject *parent)
    : QObject(parent)
{
}

void UserCompleterModelTest::shouldHaveDefaultValue()
{
    UserCompleterModel w;
    QSignalSpy rowInsertedSpy(&w, &UserCompleterModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &UserCompleterModel::rowsAboutToBeInserted);
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);

    QHash<int, QByteArray> roles;
    roles[UserCompleterModel::UserName] = QByteArrayLiteral("username");
    roles[UserCompleterModel::UserId] = QByteArrayLiteral("userid");
    roles[UserCompleterModel::UserIconStatus] = QByteArrayLiteral("iconstatus");
    QCOMPARE(w.roleNames(), roles);
}

void UserCompleterModelTest::shouldAddValues()
{
    UserCompleterModel w;
    QVector<User *> users;
    QSignalSpy rowInsertedSpy(&w, &UserCompleterModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &UserCompleterModel::rowsAboutToBeInserted);
    for (int i = 0; i < 10; i++) {
        User *user = new User;
        user->setName(QStringLiteral("name%1").arg(i));
        user->setStatus(QStringLiteral("status%1").arg(i));
        user->setUserId(QStringLiteral("userId%1").arg(i));
        user->setUserName(QStringLiteral("username%1").arg(i));
        users.append(user);
    }
    w.insertUsers(users);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,9"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,9"));

    qDeleteAll(users);
    users.clear();
    for (int i = 0; i < 3; ++i) {
        User *user = new User;
        user->setName(QStringLiteral("name%1").arg(i));
        user->setStatus(QStringLiteral("status%1").arg(i));
        user->setUserId(QStringLiteral("userId%1").arg(i));
        user->setUserName(QStringLiteral("username%1").arg(i));
        users.append(user);
    }
    w.insertUsers(users);
    QCOMPARE(w.rowCount(), 3);
    QCOMPARE(rowInsertedSpy.count(), 2);
    QCOMPARE(rowABTInserted.count(), 2);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,9;0,2"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,9;0,2"));
}

void UserCompleterModelTest::shouldVerifyData()
{
    UserCompleterModel w;
    QVector<User *> users;
    for (int i = 0; i < 10; i++) {
        User *user = new User;
        user->setName(QStringLiteral("name%1").arg(i));
        user->setStatus(QStringLiteral("status%1").arg(i));
        user->setUserId(QStringLiteral("userId%1").arg(i));
        user->setUserName(QStringLiteral("username%1").arg(i));
        users.append(user);
    }
    w.insertUsers(users);
    QCOMPARE(w.rowCount(), 10);

    for (int i = 0; i < 10; ++i) {
        QCOMPARE(w.data(w.index(i), UserCompleterModel::UserId).toString(), QStringLiteral("userId%1").arg(i));
        QCOMPARE(w.data(w.index(i), UserCompleterModel::UserName).toString(), QStringLiteral("username%1").arg(i));
    }
}
