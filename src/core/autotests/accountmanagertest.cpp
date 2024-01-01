/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountmanagertest.h"
#include "accountmanager.h"
#include "model/rocketchataccountfilterproxymodel.h"
#include "model/rocketchataccountmodel.h"
#include "rocketchataccount.h"
#include <QTest>
QTEST_GUILESS_MAIN(AccountManagerTest)

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
        auto c = new RocketChatAccount();
        w.addAccount(c);
    }
    // We have a default account when we load account
    QCOMPARE(w.rocketChatAccountModel()->rowCount(), 11);
}

void AccountManagerTest::shouldReturnAccountFromAccountName()
{
    AccountManager w;
    QVERIFY(!w.accountFromName(QStringLiteral("DDDD")));

    auto c = new RocketChatAccount();
    QString accountName = QStringLiteral("bla");
    c->setAccountName(accountName);
    w.addAccount(c);
    QVERIFY(w.accountFromName(accountName));

    QVERIFY(!w.accountFromName(QStringLiteral("DDDD")));

    accountName = QStringLiteral("bli");
    c = new RocketChatAccount();
    c->setAccountName(accountName);
    w.addAccount(c);
    QVERIFY(w.accountFromName(accountName));
    w.removeAccount(QStringLiteral("bla"));
    w.removeAccount(QStringLiteral("bli"));
}

#include "moc_accountmanagertest.cpp"
