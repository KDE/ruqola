/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountmanagertest.h"
using namespace Qt::Literals::StringLiterals;

#include "accountmanager.h"
#include "model/rocketchataccountfilterproxymodel.h"
#include "model/rocketchataccountmodel.h"
#include "rocketchataccount.h"
#include <QStandardPaths>
#include <QTest>
QTEST_GUILESS_MAIN(AccountManagerTest)

AccountManagerTest::AccountManagerTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void AccountManagerTest::shouldHaveDefaultValue()
{
    AccountManager w;
    QVERIFY(w.rocketChatAccountModel());
    QVERIFY(w.rocketChatAccountProxyModel());
    QCOMPARE(w.rocketChatAccountProxyModel()->sourceModel(), w.rocketChatAccountModel());

    QVERIFY(!w.account());
}

void AccountManagerTest::shouldAddAccount()
{
    AccountManager w;
    for (int i = 0; i < 10; ++i) {
        auto c = new RocketChatAccount();
        w.addAccount(c);
    }
    QCOMPARE(w.rocketChatAccountModel()->rowCount(), 10);
}

void AccountManagerTest::shouldReturnAccountFromAccountName()
{
    AccountManager w;
    QVERIFY(!w.accountFromName(u"DDDD"_s));

    auto c = new RocketChatAccount();
    QString accountName = u"bla"_s;
    c->setAccountName(accountName);
    w.addAccount(c);
    QVERIFY(w.accountFromName(accountName));

    QVERIFY(!w.accountFromName(u"DDDD"_s));

    accountName = u"bli"_s;
    c = new RocketChatAccount();
    c->setAccountName(accountName);
    w.addAccount(c);
    QVERIFY(w.accountFromName(accountName));
    w.removeAccount(u"bla"_s);
    w.removeAccount(u"bli"_s);
}

#include "moc_accountmanagertest.cpp"
