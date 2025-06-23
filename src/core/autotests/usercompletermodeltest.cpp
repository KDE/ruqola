/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usercompletermodeltest.h"
using namespace Qt::Literals::StringLiterals;

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
        user.setName(u"name%1"_s.arg(i));
        user.setStatus(User::PresenceStatus::Offline);
        user.setUserId(u"userId%1"_s.arg(i).toLatin1());
        user.setUserName(u"username%1"_s.arg(i));
        users.append(std::move(user));
    }
    w.addUsers(users);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), u"0,9"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), u"0,9"_s);

    users.clear();
    for (int i = 0; i < 3; ++i) {
        User user;
        user.setName(u"name%1"_s.arg(i));
        user.setStatus(User::PresenceStatus::Offline);
        user.setUserId(u"userId%1"_s.arg(i).toLatin1());
        user.setUserName(u"username%1"_s.arg(i));
        users.append(std::move(user));
    }
    w.addUsers(users);
    QCOMPARE(w.rowCount(), 3);
    QCOMPARE(rowInsertedSpy.count(), 2);
    QCOMPARE(rowABTInserted.count(), 2);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), u"0,9;0,2"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), u"0,9;0,2"_s);
}

void UserCompleterModelTest::shouldVerifyData()
{
    UserCompleterModel w;
    QList<User> users;
    for (int i = 0; i < 10; i++) {
        User user;
        user.setName(u"name%1"_s.arg(i));
        user.setStatus(User::PresenceStatus::Offline);
        user.setUserId(u"userId%1"_s.arg(i).toLatin1());
        user.setUserName(u"username%1"_s.arg(i));
        users.append(std::move(user));
    }
    w.addUsers(users);
    QCOMPARE(w.rowCount(), 10);

    for (int i = 0; i < 10; ++i) {
        QCOMPARE(w.data(w.index(i), UserCompleterModel::UserId).toString(), u"userId%1"_s.arg(i));
        QCOMPARE(w.data(w.index(i), UserCompleterModel::UserName).toString(), u"username%1"_s.arg(i));
    }
}

#include "moc_usercompletermodeltest.cpp"
