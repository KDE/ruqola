/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usercompletermodeltest.h"
#include "model/usercompletermodel.h"
#include "test_model_helpers.h"
#include <QSignalSpy>
#include <QTest>

QTEST_GUILESS_MAIN(UserCompleterModelTest)

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
}

void UserCompleterModelTest::shouldAddValues()
{
    UserCompleterModel w;
    QList<User> users;
    QSignalSpy rowInsertedSpy(&w, &UserCompleterModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &UserCompleterModel::rowsAboutToBeInserted);
    for (int i = 0; i < 10; i++) {
        User user;
        user.setName(QStringLiteral("name%1").arg(i));
        user.setStatus(User::PresenceStatus::PresenceOffline);
        user.setUserId(QStringLiteral("userId%1").arg(i));
        user.setUserName(QStringLiteral("username%1").arg(i));
        users.append(std::move(user));
    }
    w.addUsers(users);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,9"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,9"));

    users.clear();
    for (int i = 0; i < 3; ++i) {
        User user;
        user.setName(QStringLiteral("name%1").arg(i));
        user.setStatus(User::PresenceStatus::PresenceOffline);
        user.setUserId(QStringLiteral("userId%1").arg(i));
        user.setUserName(QStringLiteral("username%1").arg(i));
        users.append(std::move(user));
    }
    w.addUsers(users);
    QCOMPARE(w.rowCount(), 3);
    QCOMPARE(rowInsertedSpy.count(), 2);
    QCOMPARE(rowABTInserted.count(), 2);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,9;0,2"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,9;0,2"));
}

void UserCompleterModelTest::shouldVerifyData()
{
    UserCompleterModel w;
    QList<User> users;
    for (int i = 0; i < 10; i++) {
        User user;
        user.setName(QStringLiteral("name%1").arg(i));
        user.setStatus(User::PresenceStatus::PresenceOffline);
        user.setUserId(QStringLiteral("userId%1").arg(i));
        user.setUserName(QStringLiteral("username%1").arg(i));
        users.append(std::move(user));
    }
    w.addUsers(users);
    QCOMPARE(w.rowCount(), 10);

    for (int i = 0; i < 10; ++i) {
        QCOMPARE(w.data(w.index(i), UserCompleterModel::UserId).toString(), QStringLiteral("userId%1").arg(i));
        QCOMPARE(w.data(w.index(i), UserCompleterModel::UserName).toString(), QStringLiteral("username%1").arg(i));
    }
}

#include "moc_usercompletermodeltest.cpp"
