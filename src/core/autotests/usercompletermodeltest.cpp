/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usercompletermodeltest.h"
using namespace Qt::Literals::StringLiterals;

#include "model/usercompletermodel.h"
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
    const QSignalSpy rowInsertedSpy(&w, &UserCompleterModel::rowsInserted);
    const QSignalSpy rowABTInserted(&w, &UserCompleterModel::rowsAboutToBeInserted);
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
}

void UserCompleterModelTest::shouldAddValues()
{
    UserCompleterModel w;
    QList<User> users;
    const QSignalSpy modelAboutToBeResetSpy(&w, &UserCompleterModel::modelAboutToBeReset);
    const QSignalSpy modelResetSpy(&w, &UserCompleterModel::modelReset);
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
    QCOMPARE(modelAboutToBeResetSpy.count(), 1);
    QCOMPARE(modelResetSpy.count(), 1);

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
    QCOMPARE(modelAboutToBeResetSpy.count(), 2);
    QCOMPARE(modelResetSpy.count(), 2);
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
