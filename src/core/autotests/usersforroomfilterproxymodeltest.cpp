/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersforroomfilterproxymodeltest.h"
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

    QVector<User> users;
    for (int i = 0; i < 10; i++) {
        User user;
        user.setName(QStringLiteral("name%1").arg(i));
        user.setStatus(User::PresenceStatus::PresenceOffline);
        user.setUserId(QStringLiteral("userId%1").arg(i));
        user.setUserName(QStringLiteral("username%1").arg(i));
        users.append(std::move(user));
    }
    model.setUsers(users);
    QCOMPARE(proxy.rowCount(), 10);
    QCOMPARE(model.rowCount(), 10);
}

#include "moc_usersforroomfilterproxymodeltest.cpp"
