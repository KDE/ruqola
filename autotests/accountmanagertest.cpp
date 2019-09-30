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

#include "accountmanagertest.h"
#include "accountmanager.h"
#include "rocketchataccount.h"
#include "model/rocketchataccountmodel.h"
#include "model/rocketchataccountfilterproxymodel.h"
#include <QTest>
QTEST_MAIN(AccountManagerTest)

AccountManagerTest::AccountManagerTest(QObject *parent)
    : QObject(parent)
{
}

void AccountManagerTest::shouldHaveDefaultValue()
{
    AccountManager w;
    QVERIFY(w.rocketChatAccountModel());
    QVERIFY(w.rocketChatAccountProxyModel());
    QCOMPARE(w.rocketChatAccountProxyModel()->sourceModel(), w.rocketChatAccountModel());

    QVERIFY(w.account());
}

void AccountManagerTest::shouldAddAccount()
{
    AccountManager w;
    for (int i = 0; i < 10; ++i) {
        RocketChatAccount *c = new RocketChatAccount();
        w.addAccount(c);
    }
    //We have a default account when we load account
    QCOMPARE(w.rocketChatAccountModel()->rowCount(), 11);
}
