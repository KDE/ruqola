/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "usersforroomfilterproxymodeltest.h"
#include "model/usersforroomfilterproxymodel.h"
#include "model/usersforroommodel.h"
#include <QTest>

QTEST_MAIN(UsersForRoomFilterProxyModelTest)

UsersForRoomFilterProxyModelTest::UsersForRoomFilterProxyModelTest(QObject *parent)
    : QObject(parent)
{
}

UsersForRoomFilterProxyModelTest::~UsersForRoomFilterProxyModelTest()
{
}

void UsersForRoomFilterProxyModelTest::shouldHaveDefaultValues()
{
    UsersForRoomFilterProxyModel proxy;
    UsersForRoomModel model;
    proxy.setSourceModel(&model);
    QCOMPARE(proxy.rowCount(), 0);

    QHash<int, QByteArray> roles;
    roles[UsersForRoomModel::UserName] = QByteArrayLiteral("username");
    roles[UsersForRoomModel::Name] = QByteArrayLiteral("name");
    roles[UsersForRoomModel::UserId] = QByteArrayLiteral("userid");
    roles[UsersForRoomModel::IconStatus] = QByteArrayLiteral("iconstatus");
    roles[UsersForRoomModel::DisplayName] = QByteArrayLiteral("displayname");
    QCOMPARE(proxy.roleNames(), roles);
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
        user.setStatus(QStringLiteral("status%1").arg(i));
        user.setUserId(QStringLiteral("userId%1").arg(i));
        user.setUserName(QStringLiteral("username%1").arg(i));
        users.append(user);
    }
    model.setUsers(users);
    QCOMPARE(proxy.rowCount(), 10);
    QCOMPARE(model.rowCount(), 10);
}
