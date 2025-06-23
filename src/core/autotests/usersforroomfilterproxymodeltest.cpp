/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersforroomfilterproxymodeltest.h"
using namespace Qt::Literals::StringLiterals;

#include "model/usersforroomfilterproxymodel.h"
#include "model/usersforroommodel.h"
#include <QTest>

QTEST_GUILESS_MAIN(UsersForRoomFilterProxyModelTest)

UsersForRoomFilterProxyModelTest::UsersForRoomFilterProxyModelTest(QObject *parent)
    : QObject(parent)
{
}

UsersForRoomFilterProxyModelTest::~UsersForRoomFilterProxyModelTest() = default;

void UsersForRoomFilterProxyModelTest::shouldHaveDefaultValues()
{
    UsersForRoomFilterProxyModel proxy;
    UsersForRoomModel model;
    proxy.setSourceModel(&model);
    QCOMPARE(proxy.rowCount(), 0);
}

void UsersForRoomFilterProxyModelTest::shouldAssignValue()
{
    UsersForRoomFilterProxyModel proxy;
    UsersForRoomModel model;
    proxy.setSourceModel(&model);
    QCOMPARE(proxy.rowCount(), 0);

    QList<User> users;
    for (int i = 0; i < 10; i++) {
        User user;
        user.setName(u"name%1"_s.arg(i));
        user.setStatus(User::PresenceStatus::Offline);
        user.setUserId(u"userId%1"_s.arg(i).toLatin1());
        user.setUserName(u"username%1"_s.arg(i));
        users.append(std::move(user));
    }
    model.setUsers(users);
    QCOMPARE(proxy.rowCount(), 10);
    QCOMPARE(model.rowCount(), 10);
}

#include "moc_usersforroomfilterproxymodeltest.cpp"
